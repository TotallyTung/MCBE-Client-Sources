#pragma once

struct CaretMeasureData
{
	int  mPosition;
	bool mIsSingleline;// false|insert

	CaretMeasureData()
	{
		CaretMeasureData(0xFFFFFFFF, true);
	};

	CaretMeasureData(int position, bool singlelines)
	{
		this->mPosition = position;
		this->mIsSingleline = singlelines;
	};
};