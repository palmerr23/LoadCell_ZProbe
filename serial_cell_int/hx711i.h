#ifndef HX711i_h
#define HX711i_h
/*
 * Modified integer version of Bodge's HX711 library
 * Rp 2015
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class HX711
{
	private:
		byte PD_SCK;	// Power Down and Serial Clock Input Pin
		byte DOUT;		// Serial Data Output Pin
		byte GAIN;		// amplification factor
		long OFFSET;	// used for tare weight
		long SCALE;	// used to return weight in grams, kg, ounces, whatever

	public:
		// define clock and data pin, channel, and gain factor
		// channel selection is made by passing the appropriate gain: 128 or 64 for channel A, 32 for channel B
		// gain: 128 or 64 for channel A; channel B works with 32 gain factor only
		HX711(byte dout, byte pd_sck, byte gain = 128);

		virtual ~HX711();

		// check if HX711 is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready();

		// set the gain factor; takes effect only after a call to read_cell()
		// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
		// depending on the parameter, the channel is also set to either A or B
		void set_gain(byte gain = 128);

		// waits for the chip to be ready and returns a reading
		long read_cell();

		// returns an average reading; times = how many times to read
		long read_average(byte times = 10);

		// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
		long get_value(byte times = 1);

		// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
		// times = how many readings to do
		long get_units(byte times = 1);

		// set the OFFSET value for tare weight; times = how many times to read the tare value
		void tare(byte times = 10);

		// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
		void set_scale(long scale = 1L);

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_offset(long offset = 0);

		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();
};

#endif /* HX711_h */
