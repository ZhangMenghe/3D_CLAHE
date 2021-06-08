#ifndef DICOMLOADER_H
#define DICOMLOADER_H

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>
#include <core.h>

typedef enum {
    LOAD_DICOM = 0,
    LOAD_MASK,
    LOAD_BOTH
}mLoadTarget;

class dicomLoader {
public:
    void sendDataPrepare(int height, int width, int dims, float sh, float sw, float sd);
    bool loadData(std::string filename,unsigned char*& buffer);
    bool loadData(std::string dirpath, bool wmask);

    // Getters
	GLuint GetTextureID()			{ return _textureID; }
	GLuint GetMaskID()				{ return _maskID; }
	glm::vec3 GetSize()				{ return glm::vec3(g_vol_w, g_vol_h, g_vol_depth); }
	glm::vec3 GetImageDimensions()	{ return  glm::vec3(g_img_w, g_img_h, g_img_d); }
    // uint16_t* GetImageData()        { return g_VolumeTexData; }

	// double GetMinPixelValue()		{ return _minPixelVal; }
	// double GetMaxPixelValue()		{ return _maxPixelVal; }

private:
    const static int CHANNEL_NUM = 2;

    unsigned char* g_VolumeTexData = nullptr;

    int g_img_h = 0, g_img_w = 0, g_img_d = 0;
    float g_vol_h, g_vol_w, g_vol_depth = 0;

    size_t g_vol_len;
    // size_t n_data_offset[3];
    // bool m_new_data_available = false;

    // texture ID
	GLuint _textureID, _maskID;
	// double _minPixelVal, _maxPixelVal;
};
#endif