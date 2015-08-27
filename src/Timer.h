//
//  Timer.h
//  cinderjs
//
//  Created by Sebastian Herrlinger on 26/08/15.
//
//

#ifndef __cinderjs__Timer__
#define __cinderjs__Timer__

#include <chrono>
#include <thread>
#include <functional>
#include <string.h>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include "cinder/Timer.h"
#include "cinder/ConcurrentCircularBuffer.h"
#include "v8.h"

namespace cjs {

class Timer {

  class TimerFnHolder {
    public:
    uint32_t id;
    double after;
    double scheduledAt;
    std::function<void(boost::any passOn)> fn;
    bool _repeat = false;
    bool _remove = false;
    boost::any passOn;
  };
  typedef std::shared_ptr<TimerFnHolder> TimerFn;
  
  // Used to cancel a waiting thread after timeout if not needed anymore.
  // Maybe there is a better way to interrupt the waiting thread.
  class WaitInfo {
    public:
    double waitFor;
    bool canceled = false;
  };
  
  public:
    Timer() : _timerQueue(1024) {
      _scheduleTimer = new cinder::Timer(true);
      _timerThread = std::make_shared<std::thread>( boost::bind( &Timer::_timerThreadFn, this) );
    }
    ~Timer(){
      mShouldQuit = true;
      
      _timerRun = true;
      cvTimerThread.notify_one();
    
      _timerQueue.cancel();
      
      // Shutdown Timer Thread
      if( _timerThread ) {
        _timerThread->join();
        _timerThread.reset();
      }
      
      if( waitThread ){
        waitThread->join();
        waitThread.reset();
      }
    }
  
    int set( int timeout, std::function<void(boost::any passOn)> callback, bool repeat, boost::any passOn);
    void clear( uint32_t id );
  
  private:
    std::mutex _mutex;
    std::shared_ptr<std::thread> _timerThread;
    std::condition_variable cvTimerThread;
    volatile bool _timerRun = false;
    cinder::ConcurrentCircularBuffer<TimerFn> _timerQueue;
    cinder::Timer _scheduleTimer;
    double now;
    std::map<uint32_t, TimerFn> mTimerFns;
    double mNextScheduledTime = 0;
    std::shared_ptr<std::thread> waitThread;
    std::shared_ptr<WaitInfo> currentWaitInfo;
    std::vector<uint32_t> markedForDeletion;
  
    uint32_t _idCounter = 0;
  
    void _timerThreadFn();
    void _tick();
    void _timerWaitingThreadFn( std::shared_ptr<WaitInfo> waitInfo );
  
    bool mShouldQuit = false;
};
  
/**
 * Set a new timer
 * The callback function will be called after the given timeout
 * @return int timer id (with which a timer can be canceled as well)
 * TODO: make timeout cancelable
 */
int Timer::set( int timeout, std::function<void(boost::any passOn)> callback, bool repeat, boost::any passOn ) {
  TimerFn newTimeout(new TimerFnHolder());
  
  newTimeout->id = _idCounter++;
  newTimeout->after = timeout;
  newTimeout->scheduledAt = _scheduleTimer.getSeconds() * 1000 + timeout;
  newTimeout->_repeat = repeat;
  newTimeout->fn = callback;
  newTimeout->passOn = passOn;
  
  _timerQueue.pushFront(newTimeout);
  
  _timerRun = true;
  cvTimerThread.notify_one();
  
  return newTimeout->id;
}

/**
 *
 */
void Timer::clear( uint32_t id ) {
  TimerFn clearTimeout(new TimerFnHolder());
  
  clearTimeout->id = id;
  clearTimeout->_remove = true;
  _timerQueue.pushFront(clearTimeout);
  
  _timerRun = true;
  cvTimerThread.notify_one();
}

/**
 *
 */
void Timer::_timerWaitingThreadFn( std::shared_ptr<WaitInfo> info ){
  cinder::ThreadSetup threadSetup;
  
  if(info->waitFor > 1){
    std::this_thread::sleep_for(std::chrono::milliseconds((long)info->waitFor));
  }
  
  if(!info->canceled){
    _timerRun = true;
    cvTimerThread.notify_one();
    return;
  }
}

/**
 *
 */
void Timer::_tick(){
  if(waitThread){
    // TODO: Better way to interrupt waiting thread?
    currentWaitInfo->canceled = true;
    waitThread->detach();
    waitThread.reset();
  }
  
  while(_timerQueue.isNotEmpty()){
    TimerFn queuedTimeout;
    _timerQueue.popBack(&queuedTimeout);
    
    if(queuedTimeout->_remove){
      mTimerFns.erase(queuedTimeout->id);
      continue;
    }
    
    mTimerFns[queuedTimeout->id] = queuedTimeout;
  }
  
  if(!mTimerFns.empty() && !mShouldQuit){
    
    mNextScheduledTime = 0;
    
    for( std::map<uint32_t, TimerFn>::iterator it = mTimerFns.begin(); it != mTimerFns.end(); it++ ) {
      now = _scheduleTimer.getSeconds() * 1000;

      if(it->second->scheduledAt - 1 <= now){
        // Execute Timeout Callback
        it->second->fn(it->second->passOn);
       
        // Repeat or delete?
        if(it->second->_repeat){
          it->second->scheduledAt = now + it->second->after - 1;
        } else {
          markedForDeletion.push_back(it->first);
          continue;
        }
      }
      
      if(it->second->scheduledAt < mNextScheduledTime || mNextScheduledTime == 0){
        mNextScheduledTime = it->second->scheduledAt;
      }
    }
    
    // do deletion...
    for( std::vector<uint32_t>::iterator it = markedForDeletion.begin(); it != markedForDeletion.end(); it++ ) {
      mTimerFns.erase(*it);
    }
    markedForDeletion.clear();
    
    if(mTimerFns.empty()) {
      return;
    }
    
    // wait...
    double waitFor = mNextScheduledTime - now - 1;
    if(waitFor <= 0){
      waitFor = 1;
    }
    currentWaitInfo = std::shared_ptr<WaitInfo>(new WaitInfo());
    currentWaitInfo->waitFor = waitFor;
    waitThread = std::make_shared<std::thread>( boost::bind( &Timer::_timerWaitingThreadFn, this, currentWaitInfo ) );
    
  }
}

/**
 * Timer thread
 */
void Timer::_timerThreadFn(){
  cinder::ThreadSetup threadSetup;
  
  // Thread loop
  while( !mShouldQuit ) {
    std::unique_lock<std::mutex> lck( _mutex );
    cvTimerThread.wait(lck, [this]{ return _timerRun; });
    
    _tick();
    
    _timerRun = false;
  }
}


} // end namespace cjs

#endif /* defined(__cinderjs__Timer__) */
