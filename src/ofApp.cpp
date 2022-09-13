#include "ofApp.h"
#include "mathz.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofEnableAlphaBlending();
    ofSetCircleResolution(20);
    camera.setPosition(0, 0, -500);
    camera.lookAt({0, 0, 0});

    ofEnableSmoothing();

    fbo.allocate(w, h, GL_RGBA, 8);

    pix.allocate(w, h, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update()
{
    camera.dolly(-3.5);

    //  creating lines
    int start_index, end_index;
    float static frame, frame_random = 0;
    if (ofGetFrameNum() - frame > frame_random && lines.size() < 60)
    {
        cout << lines.size() << endl;
        frame = ofGetFrameNum();
        frame_random = ofRandom(0, 20);
        lines.push_back(ofPolyline());
        alpha.push_back(100);
        scalar.push_back(ofRandom(.05, .2));
        matrix.push_back(ofMatrix4x4());
        color.push_back(ofColor(palette[floor(ofRandom(0, 2))], alpha.back()));
        c++;
        counter.push_back(c);
        // possible endpoints
        glm::vec3 endpoints[4] = {
            {ofRandom(-len / 2, len / 2), -len, camera.getZ() + dist}, // TOP
            {ofRandom(-len / 2, len / 2), len, camera.getZ() + dist},  // BOTTOM
            {-len, ofRandom(-len / 2, len / 2), camera.getZ() + dist}, // LEFT
            {len, ofRandom(-len / 2, len / 2), camera.getZ() + dist}   // RIGHT
        };

        start_index = floor(ofRandom(0, 4));
        int r = rand() % 100;
        if (start_index == 0 || start_index == 1)
        {
            if (r < 25)
            {
                end_index = floor(ofRandom(2, 4));
            }
            else
            {
                end_index = (start_index + 1) % 2;
            }
        }
        else
        {
            if (r < 25)
            {
                end_index = floor(ofRandom(0, 2));
            }
            else
            {
                end_index = (start_index - 1) % 2 + 2;
            }
        }

        // creating first and last vertices positions
        emitter.push_back(endpoints[start_index]);
        end.push_back(endpoints[end_index]);

        lines.back().addVertex(emitter.back());
    }

    // loop every line and apply changes
    for (int i = 0; i < (int)lines.size(); i++)
    {
        lines[i].addVertex({emitter[i]});
        if (end[i].y == -len || end[i].y == len) // BOT OR TOP
        {
            end[i].x += ofSignedNoise(scalar[i], ofGetFrameNum() / 10) * 1000;
        }
        else
        {
            end[i].y += ofSignedNoise(scalar[i] * 100, ofGetFrameNum() / 10) * 1000;
        }
        glm::vec3 direction = glm::normalize(end[i] - emitter[i]);
        float length = 50;
        emitter[i] += direction * length;
        matrix[i].glRotate(scalar[i], 0, 0, 1);

        if (end[i].z < camera.getZ() + 50)
        {
            alpha[i] -= 2;
            color[i] = (ofColor(color[i], alpha[i]));
        }

        if (alpha[i] <= 0)
        {
            lines.erase(lines.begin());
            alpha.erase(alpha.begin());
            emitter.erase(emitter.begin());
            end.erase(end.begin());
            scalar.erase(scalar.begin());
            matrix.erase(matrix.begin());
            color.erase(color.begin());
        }
    }

    showFps();

    // recorder.record("/home/ezequiel/Videos/flowfield", 18);
    if (ofGetFrameNum() > 2000)
    {
        ofExit();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{

    float f = ofGetElapsedTimef() * 60 / (ofGetFrameNum() * 3 + 1);
    int n = ofNoise(f) * 200;

    fbo.begin();
    ofClear(0, 0, 0, 0);
    ofBackgroundGradient(palette[2] - n, 0, OF_GRADIENT_CIRCULAR);
    for (int j = 0; j < (int)lines.size(); j++) // debería ser al reves para dibujar las lineas nuevas atras
    {
        camera.begin();
        ofMultMatrix(matrix[j]);
        ofSetColor(color[j]);
        float lineWidth = ofMap(camera.getZ() - emitter[j].z, -300, 0, .5, 3);
        ofSetLineWidth(lineWidth);

        // draw normals
        float width = h / 100;
        float density = 1.2f * lines[j].getPerimeter();
        for (int i = 0; i < density; i++)
        {
            float length = width - width * i / density; // decreasing
            glm::vec3 point = lines[j].getPointAtPercent(i / density);

            // avoid to draw outside screen objects
            glm::vec3 pt_screen = point; // camera.worldToScreen(point);
            glm::mat4 m = matrix[j];
            glm::rotate(m, scalar[j], glm::vec3(0, 0, 1));
            glm::vec4 v = {pt_screen, 1.f};
            pt_screen = m * v;
            pt_screen = camera.worldToScreen(pt_screen);
            if (pt_screen.x > -100 && pt_screen.x < w + 100 &&
                pt_screen.y > -100 && pt_screen.y < h + 100)
            {
                float index = lines[j].getIndexAtPercent(i / density);
                glm::vec3 normal = (lines[j].getNormalAtIndexInterpolated(index));
                normal = glm::normalize(normal); // * length;
                point += normal * ofSignedNoise(i / 50.f, scalar[j] * 50, ofGetElapsedTimef() / 100.f) * 25;
                ofNoFill();
                ofDrawCircle(point.x, point.y, point.z, length);
            }
        }

        camera.end();
    }
    fbo.end();
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    fbo.readToPixels(pix);
    recorder.recordPix(pix, "/home/ezequiel/Videos/flowfield", 18);
}

void ofApp::exit()
{
    recorder.stopRecording();
}
// //--------------------------------------------------------------
// void ofApp::keyPressed(int key)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::keyReleased(int key)
// {
// }
//--------------------------------------------------------------
// void ofApp::mouseMoved(int x, int y)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::mouseDragged(int x, int y, int button)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::mousePressed(int x, int y, int button)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::mouseReleased(int x, int y, int button)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::mouseEntered(int x, int y)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::mouseExited(int x, int y)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::windowResized(int w, int h)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::gotMessage(ofMessage msg)
// {
// }
//
// //--------------------------------------------------------------
// void ofApp::dragEvent(ofDragInfo dragInfo)
// {
// }
