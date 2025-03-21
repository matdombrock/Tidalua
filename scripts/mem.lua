local cur = mem_get(1)
if cur == 0 then
  cur = 440
end
freq(cur)
if tick % 64 == 0 then
  cur = cur + 100
  if cur > 1000 then
    cur = 0
  end
  mem_set(cur, 1)
end
