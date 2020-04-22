#ifndef FOURIERDRAW_H
#define FOURIERDRAW_H

#endif // FOURIERDRAW_H

#include <complex>
#include "global_types/musicfile.h"
#include <math.h>
#include "graphics/visualization.h"
#include "player_core/player.h"
#include "static_classes/fileassistant.h"

class FourierDraw : public Visualization {
    double const dpi = acos(-1) * 2;
    MusicFile* file;
    Player *player;
    GLuint programID;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLfloat* g_vertex_buffer_data;
public:
    FourierDraw(OGLF * f);
    void init() override;
    void deInit() override;
    void update() override;
    void setMusicFile(MusicFile* f) override;
    void setPlayer(Player *pl) override;
private:
    double getY(double x, bool isRightChannel);
    double integral(double a, double freq, bool isRightChannel);
    double* GetFrame(double x, int count);
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    double Normalize(double x);
};
