note("C4", 0)
amp(0, 1)
local length = 1
local tt = seconds % length
if tt > (length / 2) then
    note("C5", 0)
end
