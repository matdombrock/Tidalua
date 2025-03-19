-- A pan value of -1 pans fully to the left
-- A pan value of 1 pans fully to the right
-- A pan value of 0 pans to the center
-- Use a sine wave to alternate the pan value between -1 -> 1
pan(math.sin(seconds * 2))
