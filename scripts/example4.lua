-- Set the waveform of the first oscillator to a saw
-- 0 = off
-- 1 = sine
-- 2 = square
-- 3 = sawtooth
wave(3)
-- Create a simple tremolo effect by modulating the amplitude of the first oscillator
amp(0.5 + (math.sin(sample_num / 8) * 0.4))
