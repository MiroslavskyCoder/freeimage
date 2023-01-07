
#ifndef _RESIZE_H_
#define _RESIZE_H_

#include "FreeImage.h"
#include "Utilities.h"
#include "Filters.h" 
 
class CWeightsTable
{ 
typedef struct { 
	double *Weights; 
	unsigned Left, Right;
} Contribution;

private: 
	Contribution *m_WeightTable; 
	unsigned m_WindowSize; 
	unsigned m_LineLength;

public: 
	CWeightsTable(CGenericFilter *pFilter, unsigned uDstSize, unsigned uSrcSize);
 
	~CWeightsTable();
 
	double getWeight(unsigned dst_pos, unsigned src_pos) {
		return m_WeightTable[dst_pos].Weights[src_pos];
	}
 
	unsigned getLeftBoundary(unsigned dst_pos) {
		return m_WeightTable[dst_pos].Left;
	}
 
	unsigned getRightBoundary(unsigned dst_pos) {
		return m_WeightTable[dst_pos].Right;
	}
};
 
class CResizeEngine
{
private: 
	CGenericFilter* m_pFilter;

public:
 
	CResizeEngine(CGenericFilter* filter):m_pFilter(filter) {}
 
	virtual ~CResizeEngine() {}
 
	FIBITMAP* scale(FIBITMAP *src, unsigned dst_width, unsigned dst_height, unsigned src_left, unsigned src_top, unsigned src_width, unsigned src_height, unsigned flags);

private:
 
	void horizontalFilter(FIBITMAP * const src, const unsigned height, const unsigned src_width,
			const unsigned src_offset_x, const unsigned src_offset_y, const RGBQUAD * const src_pal,
			FIBITMAP * const dst, const unsigned dst_width);
 
	void verticalFilter(FIBITMAP * const src, const unsigned width, const unsigned src_height,
			const unsigned src_offset_x, const unsigned src_offset_y, const RGBQUAD * const src_pal,
			FIBITMAP * const dst, const unsigned dst_height);
};

#endif  