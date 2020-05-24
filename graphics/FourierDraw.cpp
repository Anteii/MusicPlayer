#include "FourierDraw.h"
#include <fstream>
#include <string>
#define n 120

FourierDraw::FourierDraw(OGLF * _f,int index) : f(_f){
    this->index = index;
}

void FourierDraw::init() {
    g_vertex_buffer_data = new GLfloat[12] {
       0.0f, 0.0f, 0.0f,
       0.8f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f,
       0.8f, 1.0f, 0.0f
    };
    f->glGenBuffers(1, &vertexbuffer);
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, g_vertex_buffer_data, GL_STATIC_DRAW);


    f->glGenVertexArrays(1, &VertexArrayID);
    f->glBindVertexArray(VertexArrayID);

    f->glEnableVertexAttribArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    f->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    programID = LoadShaders( "SimpleVertexShader.vert", "SimpleFragmentShader.frag" );
}
void FourierDraw::deInit() {
    delete[] g_vertex_buffer_data;
    delete[] currFrame;
    delete dec;
}
void FourierDraw::update() {
    if(pc->getTrackFile() != NULL) {
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        f->glEnableVertexAttribArray(0);
        f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        f->glBindVertexArray(VertexArrayID);
        f->glUseProgram(programID);

        GLuint transVec = f->glGetUniformLocation(programID, "transform");


        currFrame = dec->GetFrame(pc->getCurrentPosition() / 1000.0, n);

        GLfloat vec[3]; vec[0] = n;
        for(int i = 0; i < n; i++) {
            vec[1] = i;
            vec[2] = Normalize(currFrame[2*i]);
            f->glUniform3fv(transVec, 1, vec);
            f->glDrawArrays(GL_TRIANGLES, 0, 3);
            f->glDrawArrays(GL_TRIANGLES, 1, 3);

            vec[2] = -Normalize(currFrame[2*i + 1]);
            f->glUniform3fv(transVec, 1, vec);
            f->glDrawArrays(GL_TRIANGLES, 0, 3);
            f->glDrawArrays(GL_TRIANGLES, 1, 3);
        }
    }
}
double FourierDraw::Normalize(double x) {
    return 1.4 / (1 + exp(-x/30)) - 0.7;
    //return 0.3*log(0.1*x + 1);
}
void FourierDraw::setPlayerController(PlayerController *ctr)
{
  pc = ctr;
  if(index == 0)
    dec = new FirstFourierDecomposition(ctr);
  else dec = new FourierDecomposition(ctr);
}
GLuint FourierDraw::LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

    // Создаем шейдеры
    GLuint VertexShaderID = f->glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = f->glCreateShader(GL_FRAGMENT_SHADER);

    // Загружаем код Вершинного Шейдера из файла
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }

    // Загружаем код Фрагментного шейдера из файла
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Компилируем Вершинный шейдер
    printf("Компиляция шейдера: %sn", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    f->glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    f->glCompileShader(VertexShaderID);

    // Выполняем проверку Вершинного шейдера
    f->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    f->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
      f->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
      fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);
    }

    // Компилируем Фрагментный шейдер
    printf("Компиляция шейдера: %sn", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    f->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    f->glCompileShader(FragmentShaderID);

    // Проверяем Фрагментный шейдер
    f->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    f->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
      f->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
      fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Создаем шейдерную программу и привязываем шейдеры к ней
    fprintf(stdout, "Создаем шейдерную программу и привязываем шейдеры к нейn");
    GLuint ProgramID = f->glCreateProgram();
    f->glAttachShader(ProgramID, VertexShaderID);
    f->glAttachShader(ProgramID, FragmentShaderID);
    f->glLinkProgram(ProgramID);

    // Проверяем шейдерную программу
    f->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    f->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
      std::vector<char> ProgramErrorMessage(InfoLogLength+1);
      f->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
      fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    f->glDeleteShader(VertexShaderID);
    f->glDeleteShader(FragmentShaderID);

    return ProgramID;
}
