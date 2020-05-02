#include "FourierDraw.h"
#include <fstream>
#include <string>
#define n 120

FourierDraw::FourierDraw(OGLF * _f) : f(_f){}

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
         double* result = GetFrame(pc->getCurrentPosition() / 1000.0, n);
         GLfloat vec[3]; vec[0] = n;
         for(int i = 0; i < n; i++) {
             vec[1] = i;
             vec[2] = Normalize(result[2*i]);
             f->glUniform3fv(transVec, 1, vec);
             glDrawArrays(GL_TRIANGLES, 0, 3);
             glDrawArrays(GL_TRIANGLES, 1, 3);

             vec[2] = -Normalize(result[2*i + 1]);
             f->glUniform3fv(transVec, 1, vec);
             glDrawArrays(GL_TRIANGLES, 0, 3);
             glDrawArrays(GL_TRIANGLES, 1, 3);
         }

         delete[] result;
    }
}

void FourierDraw::setPlayerController(PlayerController *ctr)
{
  pc = ctr;
  tfile = pc->getTrackFile();
}

double FourierDraw::getY(double x, bool isRightChannel) {
    x *= pc->getTrackFile()->getSampleRate();
    if (pc->getTrackFile()->getBitsPerSample() == 8)
    {
        char* arr = pc->getTrackFile()->getData();
        if (pc->getTrackFile()->getNumChannels() == 1) return arr[(int)x];
        else if (isRightChannel) return arr[(int)x * 2 + 1];
        else return arr[(int)x * 2];
    }
    else
    {
        short int* arr = (short int*)pc->getTrackFile()->getData();
        if (pc->getTrackFile()->getNumChannels() == 1) return arr[(int)x];
        else if (isRightChannel) return arr[(int)x * 2 + 1];
        else return arr[(int)x * 2];
    }
}
double FourierDraw::integral(double a, double freq, bool isRightChannel) {
    double x = 0, y = 0;
    double b = freq == 0 ? dpi : dpi / freq;
    if(freq > 1000) b *= 4;
    else if(freq > 100) b *= 2;
    double step = b / 2000;
    b += a;
    for (double i = a; i <= b; i += step) {
        double t = (i - a) * freq;
        double Y = getY(i, isRightChannel);
        x += Y * cos(t);
        y += Y * sin(t);
    }
    return step * sqrt(x*x + y*y);
}
double* FourierDraw::GetFrame(double x, int count) {
    double C = 100;                        // 100 - минимальная частота
    double alpha = log(16000/C) / count;    // 16000 - максимальная частота
    double* arrFreq = new double[count * 2];
    for(int i = 0; i < count; i++) {
        arrFreq[i*2] = integral(x, C*exp(alpha*i), true);
        if(pc->getTrackFile()->getNumChannels() == 1) arrFreq[i*2 + 1] = arrFreq[i*2];
        else arrFreq[i*2 + 1] = integral(x, C*exp(alpha*i), false);
    }
    return arrFreq;
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
double FourierDraw::Normalize(double x) {
    return 1.4 / (1 + exp(-x/30)) - 0.7;
}
