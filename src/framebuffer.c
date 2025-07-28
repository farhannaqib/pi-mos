#include "framebuffer.h"
#include "io.h"
#include "font.h"

// The buffer must be 16-byte aligned as only the upper 28 bits of the address can be passed via the mailbox
volatile unsigned int __attribute__((aligned(16))) mbox[36];

enum {
    MBOX_REQUEST  = 0
};

enum {
    MBOX_CH_POWER = 0,
    MBOX_CH_FB    = 1,
    MBOX_CH_VUART = 2,
    MBOX_CH_VCHIQ = 3,
    MBOX_CH_LEDS  = 4,
    MBOX_CH_BTNS  = 5,
    MBOX_CH_TOUCH = 6,
    MBOX_CH_COUNT = 7,
    MBOX_CH_PROP  = 8 // Request from ARM for response by VideoCore
};

enum {
    MBOX_TAG_SETPOWER   = 0x28001,
    MBOX_TAG_SETCLKRATE = 0x38002,

    MBOX_TAG_SETPHYWH   = 0x48003,
    MBOX_TAG_SETVIRTWH  = 0x48004,
    MBOX_TAG_SETVIRTOFF = 0x48009,
    MBOX_TAG_SETDEPTH   = 0x48005,
    MBOX_TAG_SETPXLORDR = 0x48006,
    MBOX_TAG_GETFB      = 0x40001,
    MBOX_TAG_GETPITCH   = 0x40008,

    MBOX_TAG_LAST       = 0
};

enum {
    VIDEOCORE_MBOX = (PERIPHERAL_BASE + 0x0000B880),
    MBOX_READ      = (VIDEOCORE_MBOX + 0x0),
    MBOX_POLL      = (VIDEOCORE_MBOX + 0x10),
    MBOX_SENDER    = (VIDEOCORE_MBOX + 0x14),
    MBOX_STATUS    = (VIDEOCORE_MBOX + 0x18),
    MBOX_CONFIG    = (VIDEOCORE_MBOX + 0x1C),
    MBOX_WRITE     = (VIDEOCORE_MBOX + 0x20),
    MBOX_RESPONSE  = 0x80000000,
    MBOX_FULL      = 0x80000000,
    MBOX_EMPTY     = 0x40000000
};

unsigned int mbox_call(unsigned char ch) {
    unsigned int r = ((unsigned int)((long) &mbox) &~ 0xF) | (ch & 0xF);

    while (mmio_read(MBOX_STATUS) & MBOX_FULL);

    mmio_write(MBOX_WRITE, r);

    while (1) {
        // Is there a reply?
        while (mmio_read(MBOX_STATUS) & MBOX_EMPTY);

        // Is it a reply to our message?
        if (r == mmio_read(MBOX_READ)) return mbox[1]==MBOX_RESPONSE; // Is it successful?
    }
    return 0;
}

unsigned int width, height, pitch, isrgb;
unsigned char *fb;

void fb_init() {
    mbox[0] = 35*4;
    mbox[1] = 0;

    mbox[2] = MBOX_TAG_SETPHYWH; // Tag identifier
    mbox[3] = 8; // Value size in bytes
    mbox[4] = 0;
    mbox[5] = 1920; // Value(width)
    mbox[6] = 1080; // Value(height)

    mbox[7] = MBOX_TAG_SETVIRTWH;
    mbox[8] = 8;
    mbox[9] = 0;
    mbox[10] = 1920;
    mbox[11] = 1080;

    mbox[12] = MBOX_TAG_SETVIRTOFF;
    mbox[13] = 8;
    mbox[14] = 0;
    mbox[15] = 0; // Value(x)
    mbox[16] = 0; // Value(y)

    mbox[17] = MBOX_TAG_SETDEPTH;
    mbox[18] = 4;
    mbox[19] = 0;
    mbox[20] = 32; // Bits per pixel

    mbox[21] = MBOX_TAG_SETPXLORDR;
    mbox[22] = 4;
    mbox[23] = 0;
    mbox[24] = 1; // RGB

    mbox[25] = MBOX_TAG_GETFB;
    mbox[26] = 8;
    mbox[27] = 0;
    mbox[28] = 4096; // FrameBufferInfo.pointer
    mbox[29] = 0;    // FrameBufferInfo.size

    mbox[30] = MBOX_TAG_GETPITCH;
    mbox[31] = 4;
    mbox[32] = 0;
    mbox[33] = 0; // Bytes per line

    mbox[34] = MBOX_TAG_LAST;

    // Check call is successful and we have a pointer with depth 32
    if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0) {
        mbox[28] &= 0x3FFFFFFF; // Convert GPU address to ARM address
        width = mbox[10];       // Actual physical width
        height = mbox[11];      // Actual physical height
        pitch = mbox[33];       // Number of bytes per line
        isrgb = mbox[24];       // Pixel order
        fb = (unsigned char *)((long)mbox[28]);
    }
}

void draw_pixel(int x, int y, unsigned int color) {
    int offs = (y * pitch) + (x * 4);
    *(unsigned int *)(fb + offs) = color;
}

void draw_char(unsigned char ch, int x, int y, unsigned int color, int scale) {
    unsigned char *glyph = font[ch];
    for (int i = 0; i < FONT_HEIGHT*scale; i++) {
        for (int j = 0; j < FONT_WIDTH*scale; j++) {
            unsigned int col = (*glyph & (1 << (j/scale))) ? color : 0;
            draw_pixel(x + j, y + i, col);
        }
        glyph += ((i+1)%scale) ? 0 : FONT_BPL;
    }
}

void draw_string(int x, int y, char* s, unsigned int color, int scale) {
    for (int x_=x, y_=y; *s != '\0'; s++) {
        if (*s == '\r') x_ = x;
        else if (*s == '\n') { x_ = x; y_ += FONT_HEIGHT * scale; }
        else {
            draw_char(*s, x_, y_, color, scale);
            x_ += FONT_WIDTH * scale;
        } 
    }
}

// i should rewrite this in a debug thing or in io but whatever
void itoa(char* buf, unsigned int x) {
    for (int i = 2; i >= 0; i--) {
        buf[i] = '0' + (x % 10);
        x /= 10;
    }
    buf[3] = '\0';
}

void run_shell() {
    static int x = 50;
    static int y = 20;
    unsigned char ch = uart_recv();
    char buf[4] = {0};
    itoa(buf, ch);
    //uart_write_text(buf);
    //uart_write_char('\n');

    if (ch == '\n' || ch == '\r') {
        x = 50;
        y += 20;
    }
    else if (ch == 127) {
        x -= 16;
    }
    else {
        draw_char(ch, x, y, 0x00FFFFFF, 2);
        // uart_write_char(ch);
        x += 16;
    }              
}
