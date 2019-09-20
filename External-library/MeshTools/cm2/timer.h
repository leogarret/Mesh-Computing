/**
   \file       timer.h
   \brief      Simple timer class to measure elapsed user time (wall clock).
   \copyright  1999-2019, Computing Objects, France. www.computing-objects.com

   $Rev: 3438 $
   $Date: 2019-01-07 16:51:25 +0100 (lun., 07 janv. 2019) $ 
   */

/******************************************************************************************
   This program is not free software. It is the sole property of Computing Objects, France.
   You are allowed to modify it, to recompile it, to port it on several platforms,
   but not to redistribute it, even the modified version.
 ******************************************************************************************/

#ifndef __CM2_TIMER_H__
#define __CM2_TIMER_H__


#include <sys/types.h> 
#if (defined(WIN32) | defined(WIN64))
#include <sys/timeb.h> 
#else
#include <sys/time.h> 
#endif
 

namespace cm2 {

/**
   Timer based on gettimeofday.

   Works on Windows and UNIX/LINUX.
   */
class timer 
{

public:

///
typedef timer        self_type;

/**
   Default constructor.
   
   The timer is not running.
   */
timer() 
   : _start(0.), _stop(0.), _is_running(false)
{ }


/// Starts the timer.
inline void
start() 
{
  _start = self_type::now();
  _is_running = true;
}

/// Same as start.
inline void
restart()         { this->start(); }


/// Stops the timer.
inline void
stop() 
{
  _stop = self_type::now();
  _is_running = false;
}

/// Resume the timer if stopped (duration between the stop and now will not be counted).
inline void
resume() 
{
   if (!_is_running)
   {
      _start = self_type::now() - this->elapsed_time();
      _is_running = true;
   }
}

/// The elapsed time in seconds since the timer has been running.
inline double
elapsed_time() const
{
   return (_is_running ? self_type::now() : _stop) - _start;
}

/// Pauses for a specified number of seconds.
static void 
sleep (double wait)
{   
   const double   TEND   = self_type::now() + wait;
   while (self_type::now() < TEND);
}

/// The current time in seconds (since EPOCH).
inline static double
now() 
{
   double      tv_sec, tv_usec;
   self_type::__gettimeofday(tv_sec, tv_usec);
   return tv_sec + 1E-6 * tv_usec;
}


/*------------------------ Protected members -----------------------*/
private:


/// Windows equivalent to UNIX/LINUX gettimeofday.
#if (defined(WIN32) | defined(WIN64))
static void __gettimeofday (double& tv_sec, double& tv_usec)
{
   struct _timeb timebuffer;
#if (_MSC_VER >= 1400) 
   ::_ftime_s(&timebuffer);
#else
   ::_ftime(&timebuffer);
#endif
   tv_sec = double(timebuffer.time);
   tv_usec = 1E3 * double(timebuffer.millitm);
}

#else
static void __gettimeofday (double& tv_sec, double& tv_usec)
{
   struct timeval    tv;
   struct timezone   tz;      // Not used.
   ::gettimeofday(&tv, &tz);
   tv_sec = double(tv.tv_sec);
   tv_usec = double(tv.tv_usec);

}
#endif
 

///
double       _start;
///
double       _stop;
////
bool         _is_running;

};

}  // end namespace cm2

#endif   
