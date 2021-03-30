///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2021, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/*********************************************************************************
 ** This sample demonstrates how to capture 3D point cloud and detected objects **
 **      with the ZED SDK and display the result in an OpenGL window. 	        **
 *********************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Numerics;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Forms;
using OpenGL;
using OpenGL.CoreUI;

namespace sl
{
    class MainWindow
    {
        GLViewer viewer;
        Camera zedCamera;
        ObjectDetectionRuntimeParameters obj_runtime_parameters;
        RuntimeParameters runtimeParameters;
        Mat point_cloud;
        Pose cam_pose;
        Resolution res;
        Objects object_frame;

        public MainWindow(string[] args)
        {
            // Set configuration parameters
            InitParameters init_params = new InitParameters();
            init_params.resolution = RESOLUTION.HD720;
            init_params.cameraFPS = 60;
            init_params.depthMode = DEPTH_MODE.ULTRA;
            init_params.coordinateUnits = UNIT.METER;
            init_params.coordinateSystem = COORDINATE_SYSTEM.RIGHT_HANDED_Y_UP;
            init_params.depthMaximumDistance = 50f;
            init_params.cameraDisableSelfCalib = true;

            parseArgs(args, ref init_params);
            // Open the camera
            zedCamera = new Camera(0);
            ERROR_CODE err = zedCamera.Open(ref init_params);

            if (err != ERROR_CODE.SUCCESS)
                Environment.Exit(-1);

            if (zedCamera.CameraModel != sl.MODEL.ZED2)
            {
                Console.WriteLine(" ERROR : Use ZED2 Camera only");
                return;
            }

            // Enable tracking (mandatory for object detection)
            Quaternion quat = Quaternion.Identity;
            Vector3 vec = Vector3.Zero;
            zedCamera.EnablePositionalTracking(ref quat, ref vec);

            runtimeParameters = new RuntimeParameters();

            // Enable the Objects detection module
            ObjectDetectionParameters obj_det_params = new ObjectDetectionParameters();
            obj_det_params.enableObjectTracking = true; // the object detection will track objects across multiple images, instead of an image-by-image basis
            obj_det_params.enable2DMask = false;
            obj_det_params.imageSync = true; // the object detection is synchronized to the image
            obj_det_params.detectionModel = sl.DETECTION_MODEL.MULTI_CLASS_BOX_ACCURATE;

            zedCamera.EnableObjectDetection(ref obj_det_params);

            // Create ZED Objects filled in the main loop
            object_frame = new Objects();
            point_cloud = new Mat();
            int Height = zedCamera.ImageHeight;
            int Width = zedCamera.ImageWidth;

            res = new Resolution((uint)Width, (uint)Height);
            point_cloud.Create(res, MAT_TYPE.MAT_32F_C4, MEM.CPU);

            // Create OpenGL Viewer
            viewer = new GLViewer();

            // Configure object detection runtime parameters
            obj_runtime_parameters = new ObjectDetectionRuntimeParameters();
            obj_runtime_parameters.detectionConfidenceThreshold = 35;
            obj_runtime_parameters.objectClassFilter = new int[(int)OBJECT_CLASS.LAST];
            obj_runtime_parameters.objectClassFilter[(int)sl.OBJECT_CLASS.PERSON] = Convert.ToInt32(true);
            //obj_runtime_parameters.objectClassFilter[(int)sl.OBJECT_CLASS.VEHICLE] = Convert.ToInt32(true);
            // To set a specific threshold
            obj_runtime_parameters.objectConfidenceThreshold = new int[(int)OBJECT_CLASS.LAST];
            obj_runtime_parameters.objectConfidenceThreshold[(int)sl.OBJECT_CLASS.PERSON] = 35;
            //obj_runtime_parameters.object_confidence_threshold[(int)sl.OBJECT_CLASS.VEHICLE] = 35;

            cam_pose = new Pose();

            // Create OpenGL window
            CreateWindow();
        }

        // Create Window
        public void CreateWindow()
        {
            using (OpenGL.CoreUI.NativeWindow nativeWindow = OpenGL.CoreUI.NativeWindow.Create())
            {
                nativeWindow.ContextCreated += NativeWindow_ContextCreated;
                nativeWindow.Render += NativeWindow_Render;
                nativeWindow.MouseMove += NativeWindow_MouseEvent;
                nativeWindow.Resize += NativeWindow_Resize;
                nativeWindow.KeyDown += (object obj, NativeWindowKeyEventArgs e) =>
                {
                    switch (e.Key)
                    {
                        case KeyCode.Escape:
                            close();
                            nativeWindow.Stop();
                            break;

                        case KeyCode.F:
                            nativeWindow.Fullscreen = !nativeWindow.Fullscreen;
                            break;
                    }

                    viewer.keyEventFunction(e);
                };

                //nativeWindow.MultisampleBits = 4;

                int wnd_h = Screen.PrimaryScreen.Bounds.Height;
                int wnd_w = Screen.PrimaryScreen.Bounds.Width;

                int height = (int)(wnd_h * 0.9f);
                int width = (int)(wnd_w * 0.9f);

                if (width > zedCamera.ImageWidth && height > zedCamera.ImageHeight)
                {
                    width = zedCamera.ImageWidth;
                    height = zedCamera.ImageHeight;
                }

                nativeWindow.Create((int)(zedCamera.ImageWidth * 0.05f), (int)(zedCamera.ImageHeight * 0.05f), 1200, 700, NativeWindowStyle.Resizeable);
                nativeWindow.Show();
                nativeWindow.Run();
            }
        }

        private void NativeWindow_Resize(object sender, EventArgs e)
        {
            OpenGL.CoreUI.NativeWindow nativeWindow = (OpenGL.CoreUI.NativeWindow)sender;

            viewer.resizeCallback((int)nativeWindow.Width, (int)nativeWindow.Height);
        }

        private void NativeWindow_MouseEvent(object sender, NativeWindowMouseEventArgs e)
        {
            viewer.mouseEventFunction(e);
            viewer.computeMouseMotion(e.Location.X, e.Location.Y);
        }

        // Init Window
        private void NativeWindow_ContextCreated(object sender, NativeWindowEventArgs e)
        {
            OpenGL.CoreUI.NativeWindow nativeWindow = (OpenGL.CoreUI.NativeWindow)sender;

            Gl.ReadBuffer(ReadBufferMode.Back);
            Gl.ClearColor(0.2f, 0.19f, 0.2f, 1.0f);

            Gl.Enable(EnableCap.DepthTest);

            Gl.Enable(EnableCap.Blend);
            Gl.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha);

            Gl.Enable(EnableCap.LineSmooth);
            Gl.Hint(HintTarget.LineSmoothHint, HintMode.Nicest);

            viewer.init(zedCamera.GetCalibrationParameters().leftCam);
        }

        // Render loop
        private void NativeWindow_Render(object sender, NativeWindowEventArgs e)
        {
            OpenGL.CoreUI.NativeWindow nativeWindow = (OpenGL.CoreUI.NativeWindow)sender;
            Gl.Viewport(0, 0, (int)nativeWindow.Width, (int)nativeWindow.Height);
            Gl.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            ERROR_CODE err = ERROR_CODE.FAILURE;
            if (viewer.isAvailable() && zedCamera.Grab(ref runtimeParameters) == ERROR_CODE.SUCCESS)
            {
                if (point_cloud.IsInit())
                {
                    // Retrieve Objects
                    err = zedCamera.RetrieveObjects(ref object_frame, ref obj_runtime_parameters);

                    if (err == ERROR_CODE.SUCCESS && object_frame.isNew != 0)
                    {
                        // Retrieve left image
                        zedCamera.RetrieveMeasure(point_cloud, sl.MEASURE.XYZRGBA, sl.MEM.CPU, res);
                        zedCamera.GetPosition(ref cam_pose, REFERENCE_FRAME.WORLD);
                        //Update GL View
                        viewer.update(point_cloud, object_frame, cam_pose);
                        viewer.render();
                    }

                }
            }
        }

        private void close()
        {
            zedCamera.DisablePositionalTracking();
            zedCamera.DisableObjectDetection();
            zedCamera.Close();
            viewer.exit();
        }

        private void parseArgs(string[] args , ref sl.InitParameters param)
        {
            if (args.Length > 0 && args[0].IndexOf(".svo") != -1)
            {
                // SVO input mode
                param.inputType = INPUT_TYPE.SVO;
                param.pathSVO = args[0];
                Console.WriteLine("[Sample] Using SVO File input: " + args[0]);
            }
            else if (args.Length > 0 && args[0].IndexOf(".svo") == -1)
            {
                IPAddress ip;
                string arg = args[0];
                if (IPAddress.TryParse(arg, out ip))
                {
                    // Stream input mode - IP + port
                    param.inputType = INPUT_TYPE.STREAM;
                    param.ipStream = ip.ToString();
                    Console.WriteLine("[Sample] Using Stream input, IP : " + ip);
                }
                else if (args[0].IndexOf("HD2K") != -1)
                {
                    param.resolution = sl.RESOLUTION.HD2K;
                    Console.WriteLine("[Sample] Using Camera in resolution HD2K");
                }
                else if (args[0].IndexOf("HD1080") != -1)
                {
                    param.resolution = sl.RESOLUTION.HD1080;
                    Console.WriteLine("[Sample] Using Camera in resolution HD1080");
                }
                else if (args[0].IndexOf("HD720") != -1)
                {
                    param.resolution = sl.RESOLUTION.HD720;
                    Console.WriteLine("[Sample] Using Camera in resolution HD720");
                }
                else if (args[0].IndexOf("VGA") != -1)
                {
                    param.resolution = sl.RESOLUTION.VGA;
                    Console.WriteLine("[Sample] Using Camera in resolution VGA");
                }
            }
            else
            {
                //
            }
        }
    }
}
