#ifndef MGUI_H
#define MGUI_H

struct NVGcontext;

int mgInit();
void mgTerminate();

enum MUImouseButton {
	MG_MOUSE_PRESSED	= 1 << 0,
	MG_MOUSE_RELEASED	= 1 << 1,
};

enum MUIoverflow {
	MG_FIT,
	MG_HIDDEN,
	MG_SCROLL,
};

enum MUIdir {
	MG_ROW,
	MG_COL,
};

enum MUIalign {
	MG_START,
	MG_END,
	MG_CENTER,
	MG_JUSTIFY,
};

#define MG_AUTO_SIZE 0xffff

enum MGargTypes {
	MG_NONE = 0,
	MG_WIDTH_ARG,
	MG_HEIGHT_ARG,
	MG_SPACING_ARG,
	MG_PADDINGX_ARG,
	MG_PADDINGY_ARG,
	MG_GROW_ARG,
	MG_ALIGN_ARG,
	MG_OVERFLOW_ARG,
	MG_FONTSIZE_ARG,
	MG_TEXTALIGN_ARG,
	MG_LOGIC_ARG,
	MG_STYLE_ARG,
	MG_TEXTCOLOR_ARG,
	MG_FILLCOLOR_ARG,
	MG_BORDERCOLOR_ARG,
	MG_BORDERSIZE_ARG,
	MG_CORNERRADIUS_ARG,
	MG_TAG_ARG,
};

enum MGlogic {
	MG_CLICK = 1,
	MG_DRAG = 2,
	MG_TYPE = 3,
};


struct MGarg {
	unsigned char arg;
	union {
		int v;
		const char* str;
	};
};

#define mgOverflow(v)			(mgPackArg(MG_OVERFLOW_ARG, (v)))
#define mgAlign(v)				(mgPackArg(MG_ALIGN_ARG, (v)))
#define mgGrow(v)				(mgPackArg(MG_GROW_ARG, (v)))
#define mgWidth(v)				(mgPackArg(MG_WIDTH_ARG, (v)))
#define mgHeight(v)				(mgPackArg(MG_HEIGHT_ARG, (v)))
#define mgPaddingX(v)			(mgPackArg(MG_PADDINGX_ARG, (v)))
#define mgPaddingY(v)			(mgPackArg(MG_PADDINGY_ARG, (v)))
#define mgPadding(x,y)			(mgPackArg(MG_PADDINGX_ARG, (x))), (mgPackArg(MG_PADDINGY_ARG, (y)))
#define mgSpacing(v)			(mgPackArg(MG_SPACING_ARG, (v)))
#define mgFontSize(v)			(mgPackArg(MG_FONTSIZE_ARG, (v)))
#define mgTextAlign(v)			(mgPackArg(MG_TEXTALIGN_ARG, (v)))
#define mgLogic(v)				(mgPackArg(MG_LOGIC_ARG, (v)))
#define mgTextColor(r,g,b,a)	(mgPackArg(MG_TEXTCOLOR_ARG, mgRGBA((r),(b),(b),(a))))
#define mgFillColor(r,g,b,a)	(mgPackArg(MG_FILLCOLOR_ARG, mgRGBA((r),(b),(b),(a))))
#define mgBorderColor(r,g,b,a)	(mgPackArg(MG_BORDERCOLOR_ARG, mgRGBA((r),(b),(b),(a))))
#define mgBorderSize(v)			(mgPackArg(MG_BORDERSIZE_ARG, (v)))
#define mgCornerRadius(v)		(mgPackArg(MG_CORNERRADIUS_ARG, (v)))
#define mgTag(v)				(mgPackStrArg(MG_TAG_ARG, (v)))

struct MGstyle {
	unsigned int set;
	unsigned int textColor;
	unsigned int fillColor;
	unsigned int borderColor;
	unsigned short width;
	unsigned short height;
	unsigned short style;
	unsigned char spacing;
	unsigned char paddingx;
	unsigned char paddingy;
	unsigned char grow;
	unsigned char align;
	unsigned char overflow;
	unsigned char fontSize;
	unsigned char textAlign;
	unsigned char logic;
	unsigned char cornerRadius;
	unsigned char borderSize;
	const char* tag;
};

#define mgStyle(...) mgStyle_(0, ##__VA_ARGS__, mgPackArg(MG_NONE,0))
struct MGstyle mgStyle_(unsigned int dummy, ...);
struct MGarg mgPackArg(unsigned char arg, int v);
struct MGarg mgPackStrArg(unsigned char arg, const char* str);
struct MGstyle mgMergeStyles(struct MGstyle dst, struct MGstyle src);
unsigned int mgRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
unsigned int mgFindStyle(const char* name);
unsigned int mgCreateStyle(const char* selector, struct MGstyle normal, struct MGstyle hover, struct MGstyle active, struct MGstyle focus);


struct MGhit {
	unsigned char clicked;
	unsigned char pressed;
	unsigned char dragged;
	unsigned char released;
	float mx, my;
	float deltamx, deltamy;
	float localmx, localmy;
	float bounds[4];
	unsigned char storage[128];
};

enum MGwidgetType {
	MG_BOX,
	MG_TEXT,
	MG_ICON,
	MG_SLIDER,
	MG_INPUT,
};

enum MGwidgetState {
	MG_NORMAL = 0,
	MG_HOVER = 1<<0,
	MG_ACTIVE = 1<<1,
	MG_FOCUS = 1<<2,
};


struct MGwidget {
	unsigned int id;
	float x, y, width, height;
	struct MGstyle style;
	unsigned char dir;
	unsigned char type;
	unsigned char state;
	union {
		struct {
			char* text;
		} text;
		struct {
			char* text;
			int maxtext;
			float value;
		} input;
		struct {
			float value;
			float vmin;
			float vmax;
		} slider;
		struct {
			struct MGwidget* children;
		} box;
	};
	struct MGwidget* next;
	struct MGwidget* parent;
};

void mgFrameBegin(struct NVGcontext* vg, int width, int height, int mx, int my, int mbut);
void mgFrameEnd();

struct MGhit* mgPanelBegin(int dir, float x, float y, float width, float height, struct MGstyle args);
struct MGhit* mgPanelEnd();

struct MGhit* mgBoxBegin(int dir, struct MGstyle args);
struct MGhit* mgBoxEnd();

struct MGhit* mgText(const char* text, struct MGstyle args);
struct MGhit* mgIcon(int width, int height, struct MGstyle args);
struct MGhit* mgSlider(float* value, float vmin, float vmax, struct MGstyle args);
struct MGhit* mgInput(char* text, int maxtext, struct MGstyle args);

// Derivative
struct MGhit* mgNumber(float* value, struct MGstyle args);
struct MGhit* mgSelect(int* value, const char** choices, int nchoises, struct MGstyle args);
struct MGhit* mgLabel(const char* text, struct MGstyle args);
struct MGhit* mgNumber3(float* x, float* y, float* z, const char* units, struct MGstyle args);
struct MGhit* mgColor(float* r, float* g, float* b, float* a, struct MGstyle args);
struct MGhit* mgCheckBox(const char* text, int* value, struct MGstyle args);
struct MGhit* mgButton(const char* text, struct MGstyle args);
struct MGhit* mgItem(const char* text, struct MGstyle args);


#endif // MGUI_H
