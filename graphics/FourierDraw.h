#ifndef FOURIERDRAW_H
#define FOURIERDRAW_H

#endif // FOURIERDRAW_H

#include "global_types/musicfile.h"
#include "graphics/visualization.h"
#include "player_core/player.h"
#include "static_classes/fileassistant.h"
#include "fft/firstfourierdecomposition.h"

class FourierDraw : public Visualization {
    int index = 0;
    FourierDecomposition* dec;
    double* currFrame;
    PlayerController * pc;
    GLuint programID;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLfloat* g_vertex_buffer_data;
public:
    FourierDraw(OGLF * f, int index);
    void init() override;
    void deInit() override;
    void update() override;
    void setPlayerController(PlayerController *ctr) override;
private:
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    double Normalize(double x);
    OGLF * f;
};
