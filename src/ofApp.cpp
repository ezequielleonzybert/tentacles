#include "ofApp.h"
#include "mathz.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofEnableAlphaBlending();
    // ofBackground(palette[3]);

    //  ofSetVerticalSync(false);
    //  ofSetFrameRate(1000);
    ofSetCircleResolution(20);
    camera.setPosition(w / 2, h / 2, -500);
    camera.lookAt({w / 2, h / 2, 0});

    fbo.allocate(w, h, GL_RGBA, 8);
    pix.allocate(w, h, OF_IMAGE_COLOR_ALPHA);

    ofSetLineWidth(1.5);
}

//--------------------------------------------------------------
void ofApp::update()
{
    camera.dolly(-0.6);

    //  creating lines
    int start_index, end_index;
    float static time;
    if (ofGetElapsedTimef() - time > ofRandom(.3, 1.7) && lines.size() < 30)
    {
        time = ofGetElapsedTimef();
        lines.push_back(ofPolyline());
        scales.push_back(1);
        alpha.push_back(100);
        scalar.push_back(ofRandom(.05, .2));
        matrix.push_back(ofMatrix4x4());
        color.push_back(ofColor(palette[floor(ofRandom(0, 2))], alpha.back()));
        c++;
        counter.push_back(c);

        // possible endpoints
        glm::vec3 endpoints[4] = {
            {ofRandomWidth(), h + 0, camera.getZ() + 300},  // BOTTOM
            {ofRandomWidth(), -0, camera.getZ() + 300},     // TOP
            {w + 0, ofRandomHeight(), camera.getZ() + 300}, // RIGHT
            {-0, ofRandomHeight(), camera.getZ() + 300}     // LEFT
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
        if (end[i].y == 0 || end[i].y == h) // BOT OR TOP
        {
            end[i].x += ofSignedNoise(scalar[i], ofGetElapsedTimef()) * 50;
        }
        else
        {
            end[i].y += ofSignedNoise(scalar[i] * 100, ofGetElapsedTimef()) * 50;
        }
        glm::vec3 direction = glm::normalize(end[i] - emitter[i]);
        float length = 10;
        emitter[i] += direction * length;
        matrix[i].glTranslate(w / 2, h / 2, 0);
        matrix[i].glRotate(scalar[i], 0, 0, 1);
        matrix[i].glTranslate(-w / 2, -h / 2, 0);

        if (end[i].z < camera.getZ() + 100)
        {
            alpha[i] -= 2;
            color[i] = (ofColor(color[i], alpha[i]));
        }

        if (alpha[i] <= 0)
        {
            lines.erase(lines.begin());
            scales.erase(scales.begin());
            alpha.erase(alpha.begin());
            emitter.erase(emitter.begin());
            end.erase(end.begin());
            scalar.erase(scalar.begin());
            matrix.erase(matrix.begin());
            color.erase(color.begin());
        }
    }

    showFps();

    recorder.record("/home/ezequiel/Videos/flowfield", 20, 1);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    int n = ofNoise(ofGetElapsedTimef()) * 200;
    ofBackgroundGradient(palette[2] - n, 0, OF_GRADIENT_CIRCULAR);
    for (int j = 0; j < (int)lines.size(); j++)
    {
        // fbo.begin();
        // fbo.clear();
        camera.begin();
        ofMultMatrix(matrix[j]);
        ofSetColor(color[j]);
        // draw normals
        float width = 4;
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
        // fbo.end();
        // fbo.readToPixels(pix);
    }
    // fbo.draw(0, 0);
    // img.setFromPixels(pix);
    // img.draw(0, 0);
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
