// ****************************************HELPER_CPP***********************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: helper.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef HELPER_CPP
#define HELPER_CPP

#include "helper.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

// -------------------------------------------------------------------------------------
// Simple 2D Ref Map
void stupid_2d::add(int y)
{
  X.push_back(y);
}

int stupid_2d::x_size()
{
  return X.size();
}

int stupid_2d::y(int x)
{
  return X.at(x);
}

void stupid_2d::clear()
{
  X.clear();
}

// -------------------------------------------------------------------------------------
// Countdown Timer

void countdown_timer::set_timer(double Current_Time_millis, int Seconds)
{
  TIME_START = Current_Time_millis;
  DURATION = Seconds * 1000;
  TIME_END = Current_Time_millis + DURATION;

  TRIGGERED = false;
  CHECKED = false;

  ACTIVE = true;
}

bool countdown_timer::is_active()
{
  return ACTIVE;
}

void countdown_timer::update(double Current_Time_millis)
{
  if (ACTIVE == true)
  {
    if (elapsed_time(Current_Time_millis) >= (long)DURATION)
    {
      if (TRIGGERED == false)
      {
        TRIGGERED = true;
      }
    }
  }
}

bool countdown_timer::is_triggered()
{
  if (ACTIVE == true)
  {
    if (TRIGGERED == true && TRIGGER_REPORTED == false)
    {
      TRIGGER_REPORTED = true;
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

void countdown_timer::end()
{
  ACTIVE = false;
  TRIGGERED = false;
  TRIGGER_REPORTED = false;
  CHECKED = false;
  TIME_START = 0;
  TIME_END = 0;
  DURATION = 0;
}

double countdown_timer::duration()
{
  return DURATION;
}

long countdown_timer::elapsed_time(double Current_Time_millis)
{
  if (ACTIVE == true)
  {
    return Current_Time_millis - TIME_START;
  }
  else
  {
    return 0;
  }
}

float countdown_timer::timer_position(double Current_Time_millis)
{
  if (ACTIVE == true)
  {
    if (DURATION <= 0)
      return 0;

    double elapsed = Current_Time_millis - TIME_START;
    float pos = (float)elapsed / (float)DURATION;

    if (pos > 1)
      return 1;
    else
      return pos;
  }
  else
  {
    return 0;
  }
}

// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// Stupid Random Generator Structure
void stupid_random::set()
{
  long number;
  int pos = 0;
  bool found = false;

  // Create seed onece.
  srand((unsigned int)time(NULL));

  // Clear Array.
  for (int x = 0; x < StuRNDsize; x++)
  {
    stupidnumbers[x] = 255;
  }
  
  // Fill the array with random numbers.
  while(pos < StuRNDsize)
  {
    // Get a number.
    number = rand() % StuRNDsize;
    number = floor(number);

    found = false;

    // If number already in list then seach for a new
    //  random number.
    for (int x = 0; x <= pos; x++)
    {
      if (number == (int)(stupidnumbers[x]))
      {
        found = true;
      }
    }

    // If number not in list then add it.
    if (found == false)
    {
      stupidnumbers[pos] =(char )(number);
      pos ++;
    }
  }
}

// Return random char from list based on seed.
char stupid_random::getB(unsigned long seed, int size)
{
  int pos = (seed % StuRNDsize);

  return (stupidnumbers[pos] % size);  
  
  }

// Return random unsigned long from list based on seed.
//  Number will evenly distributed from other numbers 
//    based on list size.  Precision is way off, but should
//    be random
// Size is the Upper Limit. Lower Limit is 0.
unsigned long stupid_random::getUL(unsigned long seed, int size)
{
  int pos = (seed % StuRNDsize);

  return (stupidnumbers[pos] % size) * (size / StuRNDsize);
}


bool VAR_CHANGE_MON::switched(int Value)
{
  if (Value == PREV_VALUE)
  {
    return false;
  }
  else
  {
    PREV_VALUE = Value;
    return true;
  }
}

// ---------------------------------------------------------------------------------------
// Hardware Monitor Class

/*
void hardware_monitor::set(bool booValue, unsigned long tmeCheckTime, int tmeLeeWay, bool isHardware)
// Prepare the switch.
{
  tmeCHANGEDETECTEDTIME = tmeCheckTime;
  tmeLEEWAY = tmeLeeWay;
  booVALUE = booValue;
  booPREVCHANGEDETECTED = false;
  ISHARDWARE = isHardware;
}

bool hardware_monitor::changed(bool booValue, unsigned long tmeCheckTime)
// Return true if the switch state change from on to off or off to on.
{
  unsigned long tmeTme = tmeCheckTime;

  // If the switch was just activated then run any set up its initial state and run
  //  any special routines.
  if (booFIRSTRUN == true)
  {
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
    tmeCHANGEDETECTEDTIME = tmeTme;
    booFIRSTRUN = false;

    if (BOOTEST == false)
    {
      return booValue;  // Comment this line out when testing
      //return false;   // Comment this line out when not testing
    }
    else
    {
      //return booValue;// Comment this line out when testing
      return false;     // Comment this line out when not testing
    }
  }
  else if (booVALUE == booValue)
  {
    booPREVCHANGEDETECTED = false;
    return false;
  }
  else if (booPREVCHANGEDETECTED == false)
  {
    tmeCHANGEDETECTEDTIME = tmeTme;
    booPREVCHANGEDETECTED = true;
    return  false;
  }
  // Only report change of status when Leeway time is passed.  This is a essentially a
  //  debouncer.
  else if (tmeTme < (tmeCHANGEDETECTEDTIME + tmeLEEWAY))
  {
    return false;
  }
  else
  {
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
    tmeCHANGEDETECTEDTIME = tmeTme;
    tmeTOGGLEDTIME = tmeTme;
    return true;
  }
}

void hardware_monitor::read(bool booValue, unsigned long tmeCheckTime)
{
  changed(booValue, tmeCheckTime);
}
*/

// ---------------------------------------------------------------------------------------
// Simple Variable to track if a TRUE value has entered.

void TRUTH_CATCH::catch_truth(bool Value)
// Sets HAS_TRUTH = true if Value is true.
{
  if (Value == true)
  {
    HAS_TRUTH = true;
  }
}

bool TRUTH_CATCH::has_truth()
// Returns true a true value was caught in the catch truth routine.
//  Resets HAS_TRUTH = false after called.
// Returns if all catch_truths were false.
{
  if (HAS_TRUTH == true)
  {
    HAS_TRUTH = false;
    return true;
  }
  else
  {
    return false;
  }
}

  void FALSE_CATCH::catch_false(bool Value)
// Sets HAS_FALSE = true if Value is false.
{
  if (Value == false)
  {
    HAS_FALSE = true;
  }
}

bool FALSE_CATCH::has_false()
// Returns true if a false value was caught in the catch false routine.
//  Resets HAS_FALSE = false after called.
// Returns false if all catch_false were true.
{
  if (HAS_FALSE == true)
  {
    HAS_FALSE = false;
    return true;
  }
  else
  {
    return false;
  }
}

void BOOL_WITH_OVERRIDE::set(bool Override_Condition, bool Override_Value, bool Redundant_Value)
{
  if (Override_Condition == true)
  {
    VALUE = Override_Value;
    OVERRIDDEN = true;
  }
  else
  {
    VALUE = Redundant_Value;
    OVERRIDDEN = false;
  }
}

bool BOOL_WITH_OVERRIDE::value()
{
  return VALUE;
}

bool BOOL_WITH_OVERRIDE::overridden()
{
  return OVERRIDDEN;
}

void BOOL_BOUNCE::set(bool Value)
{
  VALUE = Value;
  CHANGED = true;
}

bool BOOL_BOUNCE::value()
{
  return VALUE;
}

bool BOOL_BOUNCE::bounce()
{
  if (CHANGED == true)
  {
    CHANGED = false;
    return true;
  }
  else
  {
    return false;
  }
}

// ---------------------------------------------------------------------------------------
// Min Max Time Classes

double MIN_MAX_TIME_SLICE::time_created()
{
  return TIME_CREATED;
}

void MIN_MAX_TIME_SLICE::clear(double tmeFrame_Time)
{
  TIME_CREATED = tmeFrame_Time;
  ACTIVE = false;
  VALUE = 0;
  SAMPLES = 0;
  MIN_VALUE = 0;
  MAX_VALUE = 0;
}

void MIN_MAX_TIME_SLICE::store_value(float Value)
{
  VALUE = VALUE + Value;
  SAMPLES++;

  if (ACTIVE == false)
  {
    MIN_VALUE = Value;
    MAX_VALUE = Value;
    ACTIVE = true;
  }
  else
  {
    if (Value < MIN_VALUE)
    {
      MIN_VALUE = Value;
    }
    if (Value > MAX_VALUE)
    {
      MAX_VALUE = Value;
    }
  }
}

float MIN_MAX_TIME_SLICE::total()
{
  return VALUE;
}

int MIN_MAX_TIME_SLICE::samples()
{
  return SAMPLES;
}

float MIN_MAX_TIME_SLICE::mean()
{
  if (SAMPLES > 0)
  {
    return VALUE / SAMPLES;
  }
  else
  {
    return 0;
  }
}

float MIN_MAX_TIME_SLICE::min()
{
  return MIN_VALUE;
}

float MIN_MAX_TIME_SLICE::max()
{
  return MAX_VALUE;
}

// ---------------------------------------------------------------------------------------

void MIN_MAX_TIME::create()
{
  if (PROP.SLICES > 0 && (PROP.TIME_SPAN > 0  || PROP.SAMPLE_LIMITED_SPANS == true))
  {
    SLICE_TIME = PROP.TIME_SPAN / PROP.SLICES;
    ENABLED = true;
  }
}

void MIN_MAX_TIME::remove_old_expired_frames(double tmeFrame_Time)
{
  if (PROP.SAMPLE_LIMITED_SPANS == false)
  {
    if (old_expired_frames_check_time != tmeFrame_Time)
    {
      while(TIME_SLICES.empty() == false && // Not sure this routine will always pass.
            TIME_SLICES.front().time_created() + PROP.TIME_SPAN < tmeFrame_Time)
      {
        TIME_SLICES.pop_front();
      }
      old_expired_frames_check_time = tmeFrame_Time;
    }
  }
}

int MIN_MAX_TIME::slice_size()
{
  return TIME_SLICES.size();
}

int MIN_MAX_TIME::slice_size_max()
{
  return PROP.SLICES;
}

void MIN_MAX_TIME::put_value(float Value, double tmeFrame_Time)
{
  // If no slices, create new slice.
  if (TIME_SLICES.size() == 0)
  {
    MIN_MAX_TIME_SLICE new_time_slice;
    new_time_slice.clear(tmeFrame_Time);
    TIME_SLICES.push_back(new_time_slice);
    ENABLED = true;
    
    TIME_SLICE_CREATED_FRAME_TIME = tmeFrame_Time;  
  }

  if (PROP.SAMPLE_LIMITED_SPANS == false)
  {
    // Slices are time based, new slice created at when time elapsed.
    if (tmeFrame_Time > TIME_SLICE_CREATED_FRAME_TIME + SLICE_TIME)
    {
      if ((int)TIME_SLICES.size() >= PROP.SLICES)
      {
        // New slice needed and max num reached. pop top and create new in back.
        TIME_SLICES.pop_front();
        MIN_MAX_TIME_SLICE new_time_slice;
        new_time_slice.clear(tmeFrame_Time);
        create();
        TIME_SLICES.push_back(new_time_slice);
      }
      else if ((int)TIME_SLICES.size() < PROP.SLICES)
      {
        // New slice needed and max num not reached. create new in back.
        MIN_MAX_TIME_SLICE new_time_slice;
        new_time_slice.clear(tmeFrame_Time);
        create();
        TIME_SLICES.push_back(new_time_slice);
      }

      TIME_SLICE_CREATED_FRAME_TIME = tmeFrame_Time;  
    }

    remove_old_expired_frames(tmeFrame_Time);

  }
  else
  {
    // Slices disregard time and instead based on sample size.
    if (TIME_SLICES.back().samples() > PROP.SAMPLE_LIMIT)
    {
      if ((int)TIME_SLICES.size() < PROP.SLICES)
      {
        MIN_MAX_TIME_SLICE new_time_slice;
        new_time_slice.clear(tmeFrame_Time);
        TIME_SLICES.push_back(new_time_slice);
      }
      else
      {
        TIME_SLICES.pop_front();
        MIN_MAX_TIME_SLICE new_time_slice;
        new_time_slice.clear(tmeFrame_Time);
        TIME_SLICES.push_back(new_time_slice);
      }
      
      TIME_SLICE_CREATED_FRAME_TIME = tmeFrame_Time;  
    }
  }

  if (PROP.SLICES > 0)
  {
    TIME_SLICES.back().store_value(Value);
  }
}

float MIN_MAX_TIME::min_float()
{
  float min = 0;

  if (ENABLED == true)
  {
    if (TIME_SLICES.size()> 0)
    {
      min = TIME_SLICES[0].min();

      for (int x = 1; x < (int)TIME_SLICES.size(); x++)
      {
        if (min > TIME_SLICES[x].min())
        {
          min = TIME_SLICES[x].min();
        }
      }
    }
  }
  else
  {
    min = -1;
  }

  return min;
}

int MIN_MAX_TIME::min()
{
  return (int)min_float();
}

float MIN_MAX_TIME::max_float()
{
  float max = 0;

  if (ENABLED == true)
  {
    if (TIME_SLICES.size()> 0)
    {
      max = TIME_SLICES[0].max();

      for (int x = 1; x < (int)TIME_SLICES.size(); x++)
      {
        if (max < TIME_SLICES[x].max())
        {
          max = TIME_SLICES[x].max();
        }
      }
    }
  }
  else
  {
    max = -1;
  }

  return max;
}

int MIN_MAX_TIME::max()
{
  return (int)max_float();
}

float MIN_MAX_TIME::mean_float()
{
  float mean = 0;

  float total_total = 0;
  int total_samples = 0;

  if (ENABLED == true)
  {
    if (TIME_SLICES.size()> 0)
    {
      for (int x = 0; x < (int)TIME_SLICES.size(); x++)
      {
        total_total = total_total + TIME_SLICES[x].total();
        total_samples = total_samples + TIME_SLICES[x].samples();
      }

      mean = total_total / total_samples;

    }
  }
  else
  {
    mean = -1;
  }

  return mean;
}

int MIN_MAX_TIME::direction()
{
  int ret_direction = 0;

  float value_difference = 0;

  if (ENABLED == true)
  {
    if (TIME_SLICES.size()> 0)
    {
      for (int x = 1; x < (int)TIME_SLICES.size(); x++)
      {
        value_difference = value_difference + (TIME_SLICES[x].mean() - TIME_SLICES[x-1].mean());
      }

      if (value_difference > (PROP.DIRECTION_NUTRAL_RANGE * (TIME_SLICES.size() -1)))
      {
        ret_direction = 1;
      }
      else if (value_difference < (-1) * (PROP.DIRECTION_NUTRAL_RANGE * (TIME_SLICES.size() -1)))
      {
        ret_direction = -1;
      }
    }
  }
  else
  {
    ret_direction = -1;
  }

  return ret_direction;
}

// ---------------------------------------------------------------------------------------
// Impact Resistance

int IMPACT_RESISTANCE_FLOAT::collection_size()
{
  return VALUE_COLLECTION.size();
}

void IMPACT_RESISTANCE_FLOAT::set_size(int Size)
{
  PROPS.SIZE = Size;
  FIRST_RUN = true;
}

void IMPACT_RESISTANCE_FLOAT::set_alive_time(double Alive_Time)
{
  PROPS.ALIVE_TIME = Alive_Time;
}

void IMPACT_RESISTANCE_FLOAT::set_value(double Time, float Value)
{
  IMPACT_RESISTANCE_VALUE new_value;

  if (FIRST_RUN)
  {
    VALUE_COLLECTION.reserve(PROPS.SIZE);

    for (int count = VALUE_COLLECTION.size(); count < PROPS.SIZE; count++)
    {
      new_value.ENTRY_TIME = 0;
      new_value.VALUE = -1.0f;
      VALUE_COLLECTION.push_back(new_value);
    }
    FIRST_RUN = false;
  }

  LATEST_POSITION++;
  if (LATEST_POSITION >= PROPS.SIZE)
  {
    LATEST_POSITION = 0;
  }

  VALUE_COLLECTION[LATEST_POSITION].VALUE = Value;
  VALUE_COLLECTION[LATEST_POSITION].ENTRY_TIME = Time;

  CHANGED = true;
}

float IMPACT_RESISTANCE_FLOAT::latest()
{
  if (VALUE_COLLECTION.size() > 0)
  {
    return VALUE_COLLECTION[LATEST_POSITION].VALUE;
  }
  else
  {
    return -1.0f;
  }
}

float IMPACT_RESISTANCE_FLOAT::impact(double Time)
{
  //float ret_impact_value = 0.0f;
  float mean = 0.0f;
  float total = 0.0f;

  int count = 0;
  
  // Calculate Total
  if (CHANGED == true || (OLDEST_ENTRY_TIME + PROPS.ALIVE_TIME < Time))
  {
    if (VALUE_COLLECTION.size() > 0)
    {
      OLDEST_ENTRY_TIME = Time;
      
      for (int pos = 0; pos < (int)VALUE_COLLECTION.size(); pos++)
      {
        // Only add values of items not time expired.
        if (VALUE_COLLECTION[pos].ENTRY_TIME + PROPS.ALIVE_TIME > Time)
        {
          count++;
          total = total + VALUE_COLLECTION[pos].VALUE;
          if (OLDEST_ENTRY_TIME > Time)
          {
            OLDEST_ENTRY_TIME = Time;
          }
        }
      }
      
      // Calculate mean if count is > 0. Things could've been filterd out.
      if (count > 0)
      {
        mean = (total / (float)count);          
      }

      // Recalculate Mean if Emperical Rule Enabled.
      if (PROPS.EMPERICAL_RULE_ENABLE == true)
      {
        // Calculate mean without outliers.

        // Reset vars to 0
        total = 0.0f;
        count = 0;

        // Calculate Standard Deviation
        float stdev = 0.0f;

        for (int pos = 0; pos < (int)VALUE_COLLECTION.size(); pos++)
        {
          if (VALUE_COLLECTION[pos].ENTRY_TIME + PROPS.ALIVE_TIME > Time)
          {
            stdev += pow(VALUE_COLLECTION[pos].VALUE - mean, 2.0f);
          }
        }

        stdev = sqrt(stdev / VALUE_COLLECTION.size());

        // Calculate Emperical Rule
        for (int pos = 0; pos < (int)VALUE_COLLECTION.size(); pos++)
        {
          if (VALUE_COLLECTION[pos].ENTRY_TIME + PROPS.ALIVE_TIME > Time)
          {
            if(abs(VALUE_COLLECTION[pos].VALUE - mean) <= (float)PROPS.EMPERICAL_RULE_DEVIATIONS * stdev)
            {
              count++;
              total = total + VALUE_COLLECTION[pos].VALUE;
            }
          }
        }
              
        // Calculate mean if count is > 0. Things could've been filterd out.
        if (count > 0)
        {
          mean = (total / (float)count);
        }
      }

      // Exits with mean being the significant var, with or without the Emperical Rule Calc
    }

    if (count == 0)
    {
      // Either the collection size is 0 or everthing was filtered out
      // Return latest value if no values calculated
      LATEST_VALUE = latest();
    }
    else
    {
      LATEST_VALUE = mean;
    }
  }

  CHANGED = false;

  return LATEST_VALUE;
}

// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

// ---------------------------------------------------------------------------------------

// Debug Walker
float walker_float(double Time, float One_Second_Value, float Max_value)
{
  float ret_value = (float(Time) / 1000.0f) * One_Second_Value;

  if (ret_value > Max_value)
  {
    return ret_value;
  }
  else
  {
    return ret_value;
  }
}

// Random Number

int intRandomHD(int intBase)
// Generate a random number between half and double of the base
{
  int intLowerOffset = intBase / 2;
  int intUpperOffset = (intBase * 2) - intLowerOffset;
  return ((rand() % intUpperOffset) + intLowerOffset);
}

// Some time display type things
int millis_to_time_minutes(long millis_time)
// Returns minutes portion of time.
{
  return abs(millis_time/60000);
}

int millis_to_time_seconds(long millis_time)
// Returns seconds portion of time.
{
  return abs((millis_time % 60000)/1000);
}

int get_frame_interval(int Frames_Per_Second)
{
  return (1000 / Frames_Per_Second);
}

int radio_translate_to_frequency_6(float Frequency)
{
  return (int)round(Frequency / 1000.0);
}

float temperture_translate_celcius_to_fahrenheit(float celcius)
{
  return (celcius * 9.0f / 5.0f) + 32.0f;
}

float velocity_translate_kmph_to_mph(float kmph)
{
  return kmph * 0.6213711922f;
}

float velocity_translate_Miph_to_mps(float Miph)
{
  return Miph * 0.447f;
}

float velocity_translate_kmph_to_meters_per_second(float kmph)
{
  return kmph * 0.2777777778f;
}

float velocity_translate_meters_per_second_to_kmph(float mps)
{
  return mps * 3.6f;
}

float velocity_translate_knots_to_kmph(float knots)
{
  return knots * 1.852f;
}

float pressure_translate_kPa_to_inHg(float kPa)
{
  return kPa * (1.0f / 3.386389f);
}

bool emperical_mean(vector<float> &Number_List, float Deviations, 
                    int Required_Qualifying_Dataset_Count, float &Ret_Mean, 
                    float &Ret_Mean_Difference)
{
  // Return variable reset.
  Ret_Mean = 0; 
  Ret_Mean_Difference = 0;

  int count = 0;
  float total = 0.0f;

  // Get Normal Mean
  if (Number_List.size())
  {
    // Vars Used.

    for (int pos = 0; pos < (int)Number_List.size(); pos++)
    {
      total = total + Number_List[pos];
    }
        
    // Calculate mean.
    Ret_Mean = (total / (float)Number_List.size());


    // Calculate mean without outliers.

    // Reset vars to 0
    total = 0.0f;

    // Calculate Standard Deviation
    float stdev = 0.0f;

    for (int pos = 0; pos < (int)Number_List.size(); pos++)
    {
      stdev += pow(Number_List[pos] - Ret_Mean, 2.0f);
    }

    stdev = sqrt(stdev / Number_List.size());
    Ret_Mean_Difference = Deviations * stdev;

    // Calculate Emperical Rule
    for (int pos = 0; pos < (int)Number_List.size(); pos++)
    {
      if(abs(Number_List[pos] - Ret_Mean) <= Ret_Mean_Difference)
      {
        count++;
        total = total + Number_List[pos];
      }
    }
  }
                
  // Calculate mean if count is > 0.
  if (count >= Required_Qualifying_Dataset_Count)
  {
    Ret_Mean = (total / (float)count);
    return true;
  }
  else
  {
    Ret_Mean = 0;
    return false;
  }
}

float emperical_mean(vector<float> &Number_List, float Deviations)
{
  float mean = 0;
  float mean_difference = 0;
  emperical_mean(Number_List, Deviations, 0, mean, mean_difference);
  return mean;
}

FLOAT_XYZ emperical_mean_zyz(vector<FLOAT_XYZ> &Number_List, float Deviations)
{
  FLOAT_XYZ ret_mean_xyz;

  vector<float> float_number_list;

  // X
  for (int pos = 0; pos < (int)Number_List.size(); pos++)
  {
    float_number_list.push_back(Number_List[pos].X);
  }

  float X = emperical_mean(float_number_list, Deviations);

  // Y
  float_number_list.clear();

  for (int pos = 0; pos < (int)Number_List.size(); pos++)
  {
    float_number_list.push_back(Number_List[pos].Y);
  }

  float Y = emperical_mean(float_number_list, Deviations);

  // Z
  float_number_list.clear();

  for (int pos = 0; pos < (int)Number_List.size(); pos++)
  {
    float_number_list.push_back(Number_List[pos].Z);
  }

  float Z = emperical_mean(float_number_list, Deviations);

  // Return 
  ret_mean_xyz.X = X;
  ret_mean_xyz.Y = Y;
  ret_mean_xyz.Z = Z;

  return ret_mean_xyz;
}

int position_of_scale(int size, int scale, int value)
// Calculate position of value on scale (eg 100%) with size.
//  Will not return anything larger than size.
{
  int pos = size * value / scale;

  if (pos > size)
  {
    return size;
  }
  else
  {
    return pos;
  }
}

float no_roll_difference(float low_value, float high_value, float roll_value)
{
  /*
  if (low_value < high_value)
  {
    return (high_value - low_value);
  }
  else
  {
    return (high_value - (low_value - roll_value));
  }
  */

  float difference = abs(high_value - low_value);

  if (difference < (roll_value / 2.0f))
  {
    return difference;
  }
  else
  {
    if (high_value > low_value)
    {
      return (low_value + roll_value - high_value);
    }
    else
    {
      return (high_value + roll_value - low_value);
    }
  }
}

bool is_within(int Value, int Number_1, int Number_2)
{
  if (Number_1 <= Number_2)
  {
    if (Number_1 <= Value && Value <= Number_2)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (Number_2 <= Value && Value <= Number_1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool is_within(float Value, float Number_1, float Number_2)
{
  if (Number_1 <= Number_2)
  {
    if (Number_1 <= Value && Value <= Number_2)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (Number_2 <= Value && Value <= Number_1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool set_bool_with_change_notify(bool Set_Value, bool &Bool_Variable)
{
  if (Bool_Variable == Set_Value)
  {
    return false;
  }
  else
  {
    Bool_Variable = Set_Value;
    return true;
  }
}

int get_1_pos_of_int(int Number)
{
  return Number % 10;
}

int get_2_pos_of_int(int Number)
{
  return (Number % 100 - Number % 10) / 10;
}

float mod_decimal_remainder(float Value, int Mod_Value)
{
  int upper = (int)Value / Mod_Value;
  int lower = (int)Value % Mod_Value;
  return (Value - ((float)upper * 33 + (float)lower));
}

bool get_bit_value(int baseline, int bit_set_compare)
{
  bool ret_bit_on = false;

  ret_bit_on = (baseline & bit_set_compare) == bit_set_compare;

  return ret_bit_on;
}

bool bit_value(unsigned char Value, int Bit_Location)
{
  /*
  //  Skip range checks for now.
  // Ensure the bit location is within the range of 0 to 7
  if (Bit_Location < 0 || Bit_Location > 7) 
  {
      return false; // Or handle the error as needed
  }
  */

  // Shift the bit to the least significant position and mask with 1
  return (Value >> Bit_Location) & 1;
}

int two_byte_complement_signed(unsigned char byte1, unsigned char byte2) 
{
  // thank you chat gpt.
  
  // Combine the two bytes into a 16-bit value
  int value = (static_cast<int>(byte1) << 8) | byte2;

  // Check if the value is negative
  if (value & 0x8000)
  {
    // Perform two's complement conversion
    value = -((value ^ 0xFFFF) + 1);
  }

  return value;
}

string current_time()
{
  FLED_TIME_VAR time; 
  string ret_date_time = "";

  std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
  std::chrono::duration<double>  dur = tmeNow.time_since_epoch();

  time.put_seconds(dur.count());

  ret_date_time = to_string (time.get_hour()) +
                  ":" +
                  linemerge_right_justify(2, "00", to_string(time.get_minute()));

  return ret_date_time;
}

string current_date()
{
  FLED_TIME_VAR time; 
  string ret_date_time = "";

  std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
  std::chrono::duration<double>  dur = tmeNow.time_since_epoch();

  time.put_seconds(dur.count());

    ret_date_time = "day " + to_string(time.get_day()) +
                 " of month " + to_string(time.get_month()) +
                  " of year " + to_string(time.get_year());

  return ret_date_time;
}

string file_format_system_time()
{
  FLED_TIME_VAR time; 
  string ret_date_time = "";

  std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
  std::chrono::duration<double>  dur = tmeNow.time_since_epoch();

  time.put_seconds(dur.count());

  ret_date_time = linemerge_right_justify(4, "0000", to_string(time.get_year())) + 
                  linemerge_right_justify(2, "00", to_string(time.get_month())) + 
                  linemerge_right_justify(2, "00", to_string(time.get_day())) + 
                  "_" + 
                  linemerge_right_justify(2, "00", to_string(time.get_hour())) + 
                  "." + 
                  linemerge_right_justify(2, "00", to_string(time.get_minute())) + 
                  "." + 
                  linemerge_right_justify(2, "00", to_string(time.get_second()));

  return ret_date_time;
}

string file_format_system_date()
{
  FLED_TIME_VAR time; 
  string ret_date_time = "";

  std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
  std::chrono::duration<double>  dur = tmeNow.time_since_epoch();

  time.put_seconds(dur.count());

  ret_date_time = linemerge_right_justify(4, "0000", to_string(time.get_year())) + 
                  linemerge_right_justify(2, "00", to_string(time.get_month())) + 
                  linemerge_right_justify(2, "00", to_string(time.get_day()));

  return ret_date_time;
}

string file_format_system_hour_minutes_seconds()
{
  FLED_TIME_VAR time; 
  string ret_date_time = "";

  std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
  std::chrono::duration<double>  dur = tmeNow.time_since_epoch();

  time.put_seconds(dur.count());

  ret_date_time = linemerge_right_justify(2, "00", to_string(time.get_hour())) + 
                  "." + 
                  linemerge_right_justify(2, "00", to_string(time.get_minute())) + 
                  "." + 
                  linemerge_right_justify(2, "00", to_string(time.get_second()));

  return ret_date_time;
}

// ---------------------------------------------------------------------------------------

void EMPERICAL::add_value(float Value)
{
  if (FIRST_RUN)
  {
    VALUES.reserve(PROPS.VALUE_SIZE);
  }

  VALUES.push_back(Value);

  while ((int)VALUES.size() > PROPS.VALUE_SIZE)
  {
    VALUES.erase(VALUES.begin());
  }

  CHANGED = true;
  
}

float EMPERICAL::mean()
{
  if ((int)VALUES.size() > 0)
  {
    if (CHANGED)
    {
      emperical_mean(VALUES, PROPS.DEVIATIONS, 0, LATEST_MEAN, LATEST_MEAN_DIFFERENCE);

      CHANGED = false;
      
      return LATEST_MEAN;
    }
    else
    {
      return LATEST_MEAN;
    }
  }
  else
  {
    return -1.0f;
  }
}

bool SIMPLE_MAIN_LOOP_PROCESSOR_USAGE::changed()
{
  string current = "";
  time.put_seconds(dur.count());
  clock_clounter++;

  if (clock_clounter >= 100)
  {
    // tick - tock
    clock_clounter = 0;

    if (clock == "|")
    {
      clock = "/";
    }
    else if (clock == "/")
    {
      clock = "-";
    }
    else if (clock == "-")
    {
      clock = "\\";
    }
    else 
    {
      clock = "|";
    }

    tmeNow = std::chrono::system_clock::now();
    dur = tmeNow.time_since_epoch();

    time.put_seconds(dur.count());

    current = 
      "(" + clock + ") " +
      "(" + to_string(time.get_year()) + "-" + 
            to_string(time.get_month()) + "-" + 
            to_string(time.get_day()) + " " + 
            to_string(time.get_hour()) + ":" + 
            to_string(time.get_minute()) + ":" + 
            to_string(time.get_second()) + ")";

    if (current != LATEST)
    {
      LATEST = current;
      CHANGED = true;
    }
  }

  return CHANGED;
}

string SIMPLE_MAIN_LOOP_PROCESSOR_USAGE::what_is_it()
{
  CHANGED = false;
  return LATEST;
}



#endif