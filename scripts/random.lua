local choices = {
  "C", "D", "E", "G", "A",
}
local function get_note_name(octave_mod)
  local random_note = choices[math.random(1, #choices)]
  local random_octave = math.random(3, 4)
  return random_note .. random_octave + (octave_mod or 0)
end

enable(2)
enable(3)
enable(4)

atk_rel(12, 2, 12, 4)

if tick % 128 == 1 then
  note(get_note_name(), 1)
  note(get_note_name(1), 2)
end
if tick % 64 == 1 then
  note(get_note_name(2), 3)
end
if tick % 32 == 1 then
  note(get_note_name(2), 4)
end

tick_speed(0.75)
