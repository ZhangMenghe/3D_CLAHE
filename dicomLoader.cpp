#include "dicomLoader.h"
#include <cstring> //memset
#include <codecvt>
#include <locale>
#include <string>
#include <sstream>
////////////////////////////////////////////////////////////////////////////////
// Texture Helpers 

GLuint InitTexture2D(unsigned int width, unsigned int height, 
		GLenum internalFormat, GLenum format, GLenum type, GLenum filter, void* data) {

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint InitTexture3D(unsigned int width, unsigned int height, unsigned int depth,
	GLenum internalFormat, GLenum format, GLenum type, GLenum filter, void* data) {

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	// glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, format, type, data);
    glTexStorage3D(GL_TEXTURE_3D, 1, internalFormat, width, height, depth);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, format, type, data);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
							   
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);

	glBindTexture(GL_TEXTURE_3D, 0);

	return textureID;
}

void dicomLoader::sendDataPrepare(int height, int width, int dims, float sh, float sw, float sd) {
    g_img_h = height; g_img_w = width; g_img_d = dims;
    g_vol_len = width * height * dims * CHANNEL_NUM;
    g_vol_h = sh; g_vol_w = sw; g_vol_depth = sd;
    
    if (g_VolumeTexData != nullptr) { delete[]g_VolumeTexData; g_VolumeTexData = nullptr; }
	g_VolumeTexData = new unsigned char[g_vol_len];
    memset(g_VolumeTexData, 0xff, g_vol_len * sizeof(unsigned char));
}

bool dicomLoader::loadData(std::string dirpath, bool wmask) {
    bool res = loadData(dirpath + "data", g_VolumeTexData);
    if(res){
        auto vsize = int(g_vol_len / CHANNEL_NUM) ;
        uint16_t* vol_data  = new uint16_t[vsize];
        for(auto i=0, shift =0; i<vsize; i++, shift+=2){
            vol_data[i] = (((uint16_t)g_VolumeTexData[shift+1])<<8) + (uint16_t)g_VolumeTexData[shift];
        }
        _textureID = InitTexture3D(g_img_w, g_img_h, g_img_d, GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, GL_LINEAR, vol_data);
        delete[] vol_data;
    }

    if(wmask){
        unsigned char* mask_data = new unsigned char[g_vol_len * 2];
        res &= loadData(dirpath + "mask", mask_data);
        if(res)_maskID = InitTexture3D(g_img_w, g_img_h, g_img_d, GL_RG8, GL_RG, GL_UNSIGNED_BYTE, GL_LINEAR, mask_data);
	    delete[] mask_data;
    }
    return res;
}

bool dicomLoader::loadData(std::string filename, unsigned char*& data) {
    char buffer[1024];

    std::ifstream inFile(filename, std::ios::in | std::ios::binary);

    if (!inFile.is_open()){
        std::cout<<"Failed to open "<<filename<<std::endl;
        return false;
    }
    
    for (int id = 0, offset=0; !inFile.eof(); id++) {
        inFile.read(buffer, 1024);
        std::streamsize len = inFile.gcount();
        if (len == 0) continue;
        
        void* tb = data+offset;
        memcpy(tb, buffer, len);
        offset += len;
    }
    inFile.close();
    return true;
}
