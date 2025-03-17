float synth_get_sample(float phase, int osc) {
    int mode = _synth[osc].wave % 4; 
    float sample = 0.0f;
    switch (mode) {
        case 0: // Off
            sample = 0;
            break;
        case 1: // Sine
            sample = sinf(phase);
            break;
        case 2: // Square
            sample = sinf(phase) > 0 ? 1.0f : -1.0f;
            break;
        case 3: // Saw
            sample = 2.0f * (phase / (2.0f * M_PI)) - 1.0f;
            break;
        default: // Off
            sample = 0;
            break;
    }
    return sample;
}
