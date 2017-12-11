#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/CameraUi.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class TFSystemTestApp : public App {
  public:
	void setup() override;
    void setupBuffer();
    void setupShader();
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    gl::VboRef mPositions[2],mPhi[2],mTheta[2];
    gl::VaoRef mVaos[2];
    
    int current = 0;
    int next = 1;
    
    std::vector<vec3> positions;
    std::vector<float> phi,theta;
    
    ci::gl::GlslProgRef mUpdateGlsl,mRenderGlsl;
    
    uint32_t mIterationsPerFrame = 1;
    uint32_t mIterationIndex = 0;
    int total = 7000;
    
    CameraPersp                            mCam;
    CameraUi                            mCamUi;
};


void TFSystemTestApp::setup()
{
    mCam = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.01f, 1000.0f ).calcFraming( Sphere( vec3(0), 200 ) );

    mCamUi = CameraUi( &mCam, getWindow(), -1 );
    mCam.setAspectRatio(getWindowAspectRatio());
    

    
    const float azimuth = 256.0f * M_PI / total;
    const float inclination = M_PI / total;
    const float radius = 80.0f;

    for( int i = 0; i < total; ++i )
    {    // assign starting values to particles.
        float x = radius * sin( inclination * i ) * cos( azimuth * i );
        float y = radius * cos( inclination * i );
        float z = radius * sin( inclination * i ) * sin( azimuth * i );
     
        positions.push_back(vec3(x,y,z));
        phi.push_back(randFloat());
        theta.push_back(randFloat());
    }
    
    setupShader();
    setupBuffer();
  
    
}

void TFSystemTestApp::mouseDown( MouseEvent event )
{
}

void TFSystemTestApp::update()
{
    gl::ScopedGlslProg    scopeGlsl( mUpdateGlsl );
    gl::ScopedState        scopeState( GL_RASTERIZER_DISCARD, true );
    
    for( auto i = mIterationsPerFrame; i != 0; --i ) {
        // Bind the vao that has the original vbo attached,
        // these buffers will be used to read from.
        gl::ScopedVao scopedVao( mVaos[mIterationIndex & 1] );
      
        
        // We iterate our index so that we'll be using the
        // opposing buffers to capture the data
        mIterationIndex++;
        
        // Now bind our opposing buffers to the correct index
        // so that we can capture the values coming from the shader
        gl::bindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, mPositions[mIterationIndex & 1] );
        //gl::bindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, VELOCITY_INDEX, mVelocities[mIterationIndex & 1] );
        
        // Begin Transform feedback with the correct primitive,
        // In this case, we want GL_POINTS, because each vertex
        // exists by itself
        gl::beginTransformFeedback( GL_POINTS );
        // Now we issue our draw command which puts all of the
        // setup in motion and processes all the vertices
        gl::drawArrays( GL_POINTS, 0, total );
        // After that we issue an endTransformFeedback command
        // to tell OpenGL that we're finished capturing vertices
        gl::endTransformFeedback();
    }
}

void TFSystemTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    gl::ScopedVao scopeVao( mVaos[mIterationIndex & 1] );
    gl::ScopedGlslProg scopeGlsl( mRenderGlsl );
    gl::setMatrices( mCam );
    gl::setDefaultShaderVars();
    
    gl::pointSize( 4.0f);
    gl::drawArrays( GL_POINTS, 0, total );
    
}

void TFSystemTestApp::setupShader(){
    // These are the names of our out going vertices. GlslProg needs to
    // know which attributes should be captured by Transform FeedBack.
    std::vector<std::string> feedbackVaryings({
        "oPos",
        "oPhi"
    });
    
    gl::GlslProg::Format updateFormat;
    updateFormat.vertex( loadAsset( "update.glsl" ) )
    // Because we have separate buffers with which
    // to capture attributes, we're using GL_SEPERATE_ATTRIBS
    .feedbackFormat( GL_SEPARATE_ATTRIBS )
    // We also send the names of the attributes to capture
    .feedbackVaryings( feedbackVaryings );
    
    mUpdateGlsl = gl::GlslProg::create( updateFormat );

    gl::GlslProg::Format renderFormat;
    renderFormat.vertex( loadAsset( "render.vert" ) )
    .fragment( loadAsset( "render.frag" ) );
    
    mRenderGlsl = gl::GlslProg::create( renderFormat );
}

void TFSystemTestApp::setupBuffer(){
    for ( int i = 0; i < 2; i++ ) {
        mVaos[i] = gl::Vao::create();
        gl::ScopedVao scopeVao( mVaos[i] );
        {
            
            mPositions[i] = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW );
            {
                // bind and explain the vbo to your vao so that it knows how to distribute vertices to your shaders.
                gl::ScopedBuffer sccopeBuffer( mPositions[i] );
                gl::vertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) 0 );
                gl::enableVertexAttribArray( 0 );
            }
            
            mPhi[i] = gl::Vbo::create( GL_ARRAY_BUFFER, phi.size() * sizeof(float), phi.data(), GL_STATIC_DRAW );
            {
                // bind and explain the vbo to your vao so that it knows how to distribute vertices to your shaders.
                gl::ScopedBuffer sccopeBuffer( mPositions[i] );
                gl::vertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) 0 );
                gl::enableVertexAttribArray( 1 );
            }
            
            
        }
    }
}

CINDER_APP( TFSystemTestApp, RendererGl,
           [&]( App::Settings *settings ) {
               settings->setWindowSize( 1280, 720 );
           } )

/*
 
 mPhi[i] = gl::Vbo::create( GL_ARRAY_BUFFER, phi.size() * sizeof(float), phi.data(), GL_STATIC_DRAW );
 {
 // bind and explain the vbo to your vao so that it knows how to distribute vertices to your shaders.
 gl::ScopedBuffer sccopeBuffer( mPositions[i] );
 gl::vertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) 0 );
 gl::enableVertexAttribArray( 1 );
 }
 
 mTheta[i] = gl::Vbo::create( GL_ARRAY_BUFFER, theta.size() * sizeof(float), theta.data(), GL_STATIC_DRAW );
 {
 // bind and explain the vbo to your vao so that it knows how to distribute vertices to your shaders.
 gl::ScopedBuffer sccopeBuffer( mPositions[i] );
 gl::vertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) 0 );
 gl::enableVertexAttribArray( 2 );
 }
 
 */
