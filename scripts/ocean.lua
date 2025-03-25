enable(2)
enable(3)
enable(4)
enable(5)
enable(6)

wave(5, 1)
wave(5, 2)
wave(5, 3)
wave(5, 4)
wave(1, 5)
wave(1, 6)

local ampv = 5
amp(ampv, 1)
amp(ampv, 2)
amp(ampv, 3)
amp(ampv, 4)
amp(ampv, 5)
amp(ampv, 6)

note("C6", 5)
note("A5", 6)

local speed = 0.03
local speed_mod = 0.5 + math.sin(seconds) * 0.5
local speed_mod2 = 0.5 + math.sin(seconds / 7) * 0.5
local speed_modx = math.sin(speed_mod * speed_mod2)
local clamp = 0.75
if speed_modx > clamp then
  speed_modx = clamp
elseif speed_modx < -clamp then
  speed_modx = -clamp
end
speed_modx = 0.5 + speed_modx * 0.5
seconds = 16 + seconds * speed_modx * speed

local lfo1 = 1 - math.abs(math.sin(seconds * 3.3))
local lfo2 = 1 - math.abs(math.sin(seconds * 2.2))
local lfo3 = 1 - math.abs(math.sin(seconds * 1.1))

local lfo_wind = lfo1 + lfo2 + lfo3
if lfo_wind < 2 then lfo_wind = lfo_wind * 0.05 end
detune(lfo_wind, 5)
detune(lfo_wind, 6)
amp(lfo_wind * 0.008, 5)
amp(lfo_wind * 0.0085, 6)
local wind_pan = 0.5 + math.sin(seconds * 64) * 0.5
wind_pan = wind_pan * 2
wind_pan = wind_pan - 1
pan(wind_pan, 5)
pan(-wind_pan, 6)

local mid = 800
lowpass(lfo1 * mid, (lfo1/2) + 0.5, 1)
lowpass(lfo2 * mid, lfo2, 2)
lowpass(lfo3 * mid, lfo3, 3)

lowpass(200, 1, 4)

pan (0, 1)
pan (-0.5, 2)
pan (0.5, 3)
