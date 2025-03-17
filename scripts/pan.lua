local is_left = math.floor(seconds) % 2 == 0
if (is_left) then
  pan(-1)
else
  pan(1)
end
