
wave(5)
local lfo = math.sin(seconds / 2)
highpass(10000 + lfo * 10000, 1)
