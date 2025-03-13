-- This example demonstrates how to use the LFO to control the pitch of an oscillator
-- Time based effects are done by using the sample_num variable
-- The sample_num variable is the current sample number set by the C code


-- Set the "speed" of the LFO
-- This is just an arbitrary number
local speed = 2048 * 128
-- Normalize sample_num to the range 0 -> 1
-- This is also our relative position in the LFO
local norm = (sample_num % speed) / speed
-- Create a sine wave (LFO) with a period of 1
local sin = math.sin(norm * math.pi)
-- Set the pitch range to 1 -> 5 by multiplying the sine wave by 4 and adding 1
local pitch1 = 1 + sin * 4
-- Use the pitch1 value to control the pitch of the first oscillator
-- This will create an effect that sweeps the pitch up and down
pitch(pitch1, 0)
