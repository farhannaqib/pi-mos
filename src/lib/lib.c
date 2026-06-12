void itoa(char* buf, unsigned int x) {
    if (x == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    int len = 0;
    int x_copy = x;
    while (x_copy != 0) {
        x_copy /= 10;
        len++;
    }

    for (int i = len - 1; i >= 0; i--) {
        buf[i] = '0' + (x % 10);
        x /= 10;
    }
    buf[len] = '\0';
}
