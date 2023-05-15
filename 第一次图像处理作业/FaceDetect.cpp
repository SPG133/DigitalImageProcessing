#include "FaceDetect.h"
int DetectAndDisplay(Mat &frame)
{
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    if (!face_cascade.load("haarcascade_frontalface_alt_tree.xml")) //opencv�Դ�������ʶ��
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if (!eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml"))
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
        Mat faceROI = frame_gray(faces[i]);
        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale(faceROI, eyes);
        for (size_t j = 0; j < eyes.size(); j++)
        {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2,
                faces[i].y + eyes[j].y + eyes[j].height / 2);

            int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
            circle(frame, eye_center, radius, Scalar(255, 0, 0), 4);
        }
    }
    //-- Show what you got
    return 1;
}
