-- We need multiple voices to play a chord
-- Enable the second and third oscillators (voices)
-- The first oscillator is enabled by default
enable(2)
enable(3)

-- Play a C major chord
note("C4", 1)
freq(439, 2)
note("G4", 3)

solo(2)
