void vis_collect_sample(int index, float mix) {
    if (_sys.output_mode != 2) return;
    if (index % VIS_DOWNSAMPLE == 0) {
        _sys.sample_acc[_sys.sample_acc_i] = mix;
        _sys.sample_acc_i++;
    }
}

void vis_render() {
    if (_sys.output_mode != 2) return;
    if (_sys.sample_acc_i >= VIS_BUF_SIZE) {
        _sys.sample_acc_i = 0;
        // For each sample in the buffer
        for (int i = 0; i < VIS_BUF_SIZE; i++) {
            float norm = _sys.sample_acc[i];
            norm /= 2;
            norm += 0.5;
            int pos = floor(norm * VIS_WIDTH);
            // Should we stretch the visualizer? and repeat downsampled samples?
            int stretch = VIS_STRETCH ? VIS_DOWNSAMPLE : 1;
            // For each stretch
            for (int ii = 0; ii < stretch; ii++) {
                char line[VIS_WIDTH * 8] = ""; // Support wide characters
                // For each pixel in the visualizer
                for (int iii = 0; iii < VIS_WIDTH; iii++) {
                    if (iii == pos) {
                        /*printf(" ");*/
                        strcat(line, " ");
                    }
                    else if (iii > (VIS_WIDTH / 2)) {
                        /*printf("░");*/
                        strcat(line, "░");
                    }
                    else {
                        /*printf("▒");*/
                        strcat(line, "▒");

                    }
                }
                printf("%s", line);
                printf("| %f", _sys.sample_acc[i]);
                printf("\n");
            }
        }
    }
}
