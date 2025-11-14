// GlmfcnView.cpp : implementation of the CGlmfcnView class
//

#include "stdafx.h"
#include "Glmfcn.h"

#include "GlmfcnDoc.h"
#include "GlmfcnView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "glaux.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView

IMPLEMENT_DYNCREATE(CGlmfcnView, CView)

BEGIN_MESSAGE_MAP(CGlmfcnView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//	ON_COMMAND(ID_VIEW_DIFFUSECOLOR, &CGlmfcnView::OnViewDiffusecolor)
END_MESSAGE_MAP()

float spin=0.0;

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView construction/destruction

CGlmfcnView::CGlmfcnView()
{
	// Initialize sphere rotation
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_isDragging = false;
	m_lastMousePos = CPoint(0, 0);
}

CGlmfcnView::~CGlmfcnView()
{
}

BOOL CGlmfcnView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

BOOL CGlmfcnView::InitializeOpenGL()
{
	CGlmfcnDoc* pDoc = GetDocument();
    pDoc->m_pDC = new CClientDC(this);
	m_hDC = pDoc->m_pDC->GetSafeHdc();

    if ( NULL == pDoc->m_pDC ) // failure to get DC
        {
		::AfxMessageBox("Couldn't get a valid DC.");
        return FALSE;
        }

    if ( !SetupPixelFormat() )
        {
		::AfxMessageBox("SetupPixelFormat failed.\n");
        return FALSE;
        }
	m_hRC=wglGetCurrentContext();
	if (m_hRC==NULL)
    if ( 0 == (m_hRC = ::wglCreateContext( m_hDC ) ) )
        {
		::AfxMessageBox("wglCreateContext failed.");
        return FALSE;
        }


    return makeCurrent();
}

void CGlmfcnView::RenderScene()
{
	CGlmfcnDoc* pDoc = GetDocument();
	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	// Set up ambient light for shadow effect
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuseLight[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_shininess[] = { 1.0, 1.0, 1.0, 0.0 };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_SHININESS, mat_shininess);
	
	// Set global ambient light (creates shadow effect)
	GLfloat globalAmbient[] = { 0.1f, 0.2f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	
	// Enable color material for better lighting
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	// Set material properties
	GLfloat matAmbient[] = { 0.5f, 0.5f, 0.7f, 1.0f };
	GLfloat matDiffuse[] = { 0.5f, 0.5f, 0.7f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	
	// Reset modelview matrix
	glLoadIdentity();
	
	// Position the sphere in the center
	glTranslatef(1.0f, 1.0f, 0.0f);
	
	// Apply rotation
	glRotatef(m_rotationY, 1.0f, 0.0f, 0.0f); // Rotate around X axis
	glRotatef(m_rotationX, 0.0f, 1.0f, 0.0f); // Rotate around Y axis
	
	// Draw sphere using GLU
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	
	// Draw sphere with radius 0.5, 32 slices, 32 stacks
	gluSphere(quadric, 0.5, 32, 32);
	
	gluDeleteQuadric(quadric);
	
	// Disable lighting for other drawing
	glDisable(GL_LIGHTING);
}

BOOL CGlmfcnView::SetupPixelFormat()
{
    // default pixel format for a single-buffered,
    // OpenGL-supporting, hardware-accelerated, 
    // RGBA-mode format. Pass in a pointer to a different
    // pixel format if you want something else
	CGlmfcnDoc* pDoc = GetDocument();
    PIXELFORMATDESCRIPTOR pfd = 
        {
        sizeof(PIXELFORMATDESCRIPTOR),// size of this pfd
        1,                      // version number
        PFD_DRAW_TO_WINDOW |    // support window
		PFD_DOUBLEBUFFER |		// doubble buffer
        PFD_SUPPORT_OPENGL,		// support OpenGL
        PFD_TYPE_RGBA,          // RGBA type
        24,                     // 24-bit color depth
        0, 0, 0, 0, 0, 0,       // color bits ignored
        0,                      // no alpha buffer
        0,                      // shift bit ignored
        0,                      // no accumulation buffer
        0, 0, 0, 0,             // accum bits ignored
        16,                     // 16-bit z-buffer
        0,                      // no stencil buffer
        0,                      // no auxiliary buffer
        PFD_MAIN_PLANE,         // main layer
        0,                      // reserved
        0, 0, 0                 // layer masks ignored
        };

    int pixelformat;
    PIXELFORMATDESCRIPTOR* pPFDtoUse;

    // let the user override the default pixel format
//    pPFDtoUse = (0 == pPFD)? &pfd : pPFD; 
    pPFDtoUse =  &pfd ; 

    if ( 0 == (pixelformat = 
        ::ChoosePixelFormat( m_hDC, pPFDtoUse )) )
        {
    ::AfxMessageBox("ChoosePixelFormat failed.");
    return FALSE;
        }

    if ( FALSE == ::SetPixelFormat( m_hDC,
        pixelformat, pPFDtoUse ) )
        {
    ::AfxMessageBox("SetPixelFormat failed.");
        return FALSE;
        }

    return TRUE;
}

void CGlmfcnView::myReshape()
{
	CGlmfcnDoc* pDoc = GetDocument();
    // compute the aspect ratio
    // this will keep all dimension scales equal
    pDoc->m_AspectRatio = (GLfloat)pDoc->m_WindowWidth/(GLfloat)pDoc->m_WindowHeight;

    // Now, set up the viewing area-select the full client area
    ::glViewport(0, 0, pDoc->m_WindowWidth, pDoc->m_WindowHeight);

    if ( GL_NO_ERROR != ::glGetError() )
    {
    ::AfxMessageBox("Error while trying to set viewport.");
    }

    // select the projection matrix as the recipient of
    // matrix operations (there's three to choose from)
    ::glMatrixMode(GL_PROJECTION);
    // initialize the projection matrix to a pristine state
    ::glLoadIdentity();

    // select the viewing volume. You do it after you
    // get the aspect ratio and set the viewport
//    gluPerspective( 45.0f, pDoc->m_AspectRatio, 0.1f, 20.0f );

    float r=1.0f/pDoc->m_AspectRatio;

    if (pDoc->m_WindowWidth <= pDoc->m_WindowHeight) 
        glOrtho (0, 2.0, 0, 2.0*r, -10.0, 10.0);
    else 
        glOrtho (0, 2.0/r, 0, 2.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);

    // NOTE: Other commands you could have here are
    // glFrustum, which gives you much more control over
    // the perspective view, or glOrtho which is used for
    // parallel projections. No matter what you use, check
    // the error status when you set the viewing frustum!

    if ( GL_NO_ERROR != ::glGetError() )
    {
		::AfxMessageBox("Error while trying to set viewing frustum.");
		return ;
    }


    // now select the modelview matrix and clear it
    // this is the mode we do most of our calculations in
    // so we leave it as the default mode.
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
  
    // now perform a default viewing transformations
//    ::glTranslatef( 0.0f, 0.0f, -5.0f );
//    ::glRotatef( 20.0f, 1.0f, 0.0f, 0.0f );

}

BOOL CGlmfcnView::makeCurrent()
{
	CGlmfcnDoc* pDoc = GetDocument();
//	HGLRC hglrc = 	wglGetCurrentContext();
//	if (hglrc!=pDoc->m_hRC )
    if ( FALSE == ::wglMakeCurrent(m_hDC, m_hRC ) )
        {
		::AfxMessageBox("wglMakeCurrent failed.");
        return FALSE;
        }

    // specify black as clear color
    ::glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    // specify the back of the buffer as clear depth
    ::glClearDepth( 1.0f );
    // enable depth testing
    ::glEnable( GL_DEPTH_TEST );
    
return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView drawing

void CGlmfcnView::OnDraw(CDC* pDC)
{
	CGlmfcnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// Render OpenGL scene
	if (m_hDC && m_hRC) {
		makeCurrent();
		RenderScene();
		::SwapBuffers(m_hDC);
	}
}

void CGlmfcnView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_isDragging = true;
	m_lastMousePos = point;
	SetCapture(); // Capture mouse to track movement outside window
	
	CView::OnLButtonDown(nFlags, point);
}

void CGlmfcnView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isDragging) {
		m_isDragging = false;
		ReleaseCapture(); // Release mouse capture
	}
	
	CView::OnLButtonUp(nFlags, point);
}

void CGlmfcnView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_isDragging && (nFlags & MK_LBUTTON)) {
		// Calculate mouse movement delta
		int deltaX = point.x - m_lastMousePos.x;
		int deltaY = point.y - m_lastMousePos.y;
		
		// Convert mouse movement to rotation angles
		// Horizontal movement rotates around Y axis
		m_rotationX += (float)deltaX * 0.5f;
		
		// Vertical movement rotates around X axis
		m_rotationY += (float)deltaY * 0.5f;
		
		// Keep rotation angles in reasonable range (optional)
		if (m_rotationX > 360.0f) m_rotationX -= 360.0f;
		if (m_rotationX < -360.0f) m_rotationX += 360.0f;
		if (m_rotationY > 360.0f) m_rotationY -= 360.0f;
		if (m_rotationY < -360.0f) m_rotationY += 360.0f;
		
		m_lastMousePos = point;
		Invalidate(); // Redraw the scene
	}
	
	CView::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView diagnostics

#ifdef _DEBUG
void CGlmfcnView::AssertValid() const
{
	CView::AssertValid();
}

void CGlmfcnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGlmfcnDoc* CGlmfcnView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGlmfcnDoc)));
	return (CGlmfcnDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView message handlers

int CGlmfcnView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
     InitializeOpenGL();
	
	return 0;
}


void CGlmfcnView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CGlmfcnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
    if ( 0 >= cx || 0 >= cy )
        {
        return;
        }

    // save the width and height of the current window
    pDoc->m_WindowWidth = cx;
    pDoc->m_WindowHeight = cy;

	myReshape();
}


BOOL CGlmfcnView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CView::OnEraseBkgnd(pDC);
}

