# pebble-fctx-lite

This is a graphics library for the Pebble smart watch.

Provides sub-pixel accurate, anti-aliased rendering of filled shapes.  Supports circles, line segments and bezier segments, SVG paths, and SVG fonts.

### Release notes

##### v1.6.2
* Added `fctx_string_width` function.
* Fixed bug in the string width calculation in `fctx_draw_string`.

##### v1.6.1
* Emery platform support.

##### v1.6
* Renamed `fctx_set_text_size` to `fctx_set_text_em_height` and added `fctx_set_text_cap_height`.
* Added `FTextAnchorCapMiddle` and `FTextAnchorCapTop` for aligning text relative to the font cap-height.
* Requires [pebble-fctx-compiler]() v1.2 font resources (for the cap-height metadata).
* The previously undocumented `decode_utf8_byte` function has been moved into a new [pebble-utf8](https://www.npmjs.com/package/pebble-utf8) package.
* Improvements to this README.

### Notes and caveats

The library uses an even-odd fill rule.

Only filled shapes are supported.  So, to create a line, you would need to draw a thin box.  And to draw a ring, you would plot a pair of concentric circles.
[TODO: include some code snippet examples of typical drawing operations.]

Clipping is supported for AA and BW rendering, *except* that it does not produce correct results in BW rendering mode on circular displays.

### Memory

[TODO: include an analysis of memory requirements.]

### Coordinates

    typedef int32_t fixed_t;

    typedef struct FPoint {
        fixed_t x;
        fixed_t y;
    } FPoint;

    #define FIXED_POINT_SCALE 16
    #define INT_TO_FIXED(a) ((a) * FIXED_POINT_SCALE)
    #define FIXED_TO_INT(a) ((a) / FIXED_POINT_SCALE)
    #define FIXED_MULTIPLY(a, b) (((a) * (b)) / FIXED_POINT_SCALE)

    #define FPoint(x, y) ((FPoint){(x), (y)})
    #define FPointI(x, y) ((FPoint){INT_TO_FIXED(x), INT_TO_FIXED(y)})

The library works with fixed point coordinates with a scale factor of 16.  This means that `FPoint`s can address sub-pixels of 1/16th of a screen pixel.  The above declarations are just a subset of the available types, macros and functions.  Please see [`fctx.h`](include/fctx.h) for more.

### Mode selection (color platforms only)
    void fctx_enable_aa(bool enable);
    bool fctx_is_aa_enabled();

By default, color platforms will use the anti-aliased (AA) rendering path, but the 1-bit (BW) rendering path is available as an option.  Make this selection *before* calling `fctx_init_context`.  Note that clipping does not work properly in BW mode with circular frame buffers.

### Initialization and cleanup
    void fctx_init_context(FContext* fctx, GContext* gctx);
    void fctx_deinit_context(FContext* fctx);

Initialize an FContext for rendering by providing a GContext to render to.  An internal buffer will be allocated of the same dimensions as the GContext.  This buffer will be one byte per pixel on color devices with anti-aliasing enabled.  On monochrome devices, or with anti-aliasing disabled, the buffer will be just one bit per pixel.
Deinitialize the FContext when drawing is complete.

### Drawing procedure
    void fctx_begin_fill(FContext* fctx);
    void fctx_end_fill(FContext* fctx);

To draw a filled shape, call `fctx_begin_fill` then call any number of plotting or drawing functions.  Finally, call `fctx_end_fill`.  At this point, the accumulated shape will be rendered to the GContext.

### Color
    void fctx_set_fill_color(FContext* fctx, GColor c);

The current color are applied when `fctx_end_fill` is called.

### Transform
    void fctx_set_offset(FContext* fctx, FPoint offset);

The current transform state is applied at the time a `draw` function is called.

### Primitive plotting
    void fctx_plot_edge(FContext* fctx, FPoint* a, FPoint* b);

The plotting functions are the lowest level drawing functions.  They do not apply the current transform state to the coordinates.

### Path drawing
    void fctx_draw_path(FContext* fctx, FPoint* points, uint32_t num_points);

Path (i.e. polygon) drawing respects the current transform state.  It draws an array of points as a closed polygon, automatically connecting the last and first points.

### Compiled SVG path drawing
    void fctx_draw_commands(FContext* fctx, FPoint advance, void* path_data, uint16_t length);

The `advance` parameter is an offset that is applied before the regular transform state is applied.
Compiled path resources are built by the [fctx-compiler](#resource-compiler) tool.

### Text drawing
    void fctx_set_text_em_height(FContext* fctx, FFont* font, int16_t pixels);
    void fctx_draw_string(FContext* fctx, const char* text, FFont* font, GTextAlignment alignment, FTextAnchor anchor);

The `fctx_set_text_em_height` function is a convenience method that calls `fctx_set_scale` with values to achieve a specific text em-height size (in pixels).

### Fonts
    FFont* ffont_create_from_resource(uint32_t resource_id);
    void ffont_destroy(FFont* font);

The font resources are built by the [fctx-compiler](#resource-compiler) tool.

## Resource Compiler

The `pebble-fctx-compiler` package is available for the compilation of SVG data files into a binary format for use with the pebble-fctx drawing library.

This package should be installed as a devDependency with the command:

    npm install pebble-fctx-compiler --save-dev

Then, to invoke the compiler:

    ./node_modules/.bin/fctx-compiler <svg file>

See the [README](https://github.com/jrmobley/pebble-fctx-compiler) for more info.
