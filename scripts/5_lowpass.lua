wave(5)
local lfo = math.sin(seconds / 2)
lowpass(10000 + lfo * 10000, 1)
