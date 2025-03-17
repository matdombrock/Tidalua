-- we have 2 oscillators to play with
-- The pitch starts at 220Hz
-- The value set in the pitch function multiplies the base frequency
-- Set the pitch of the first oscillator to 1.5 to get 330Hz
pitch(1.5, 0)

-- The second oscillator is set to off by default
-- An oscillator can be turned off by settings its wave to 0
-- Turn it on by setting its wave to 1
wave(1, 1)
-- Set the pitch of the second oscillator to 3
pitch(3, 1)

-- The second oscillator is a bit loud so turn it down a bit
amp(0.5, 1)
