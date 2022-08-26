#include "ofApp.h"
#include "mathz.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofEnableAlphaBlending();
    ofBackground(0);
    // ofSetVerticalSync(false);
    // ofSetFrameRate(1000);
    ofSetCircleResolution(20);
    camera.setPosition(w / 2, h / 2, -500);
    camera.lookAt({w / 2, h / 2, 0});

    ofNoFill();
    ofSetLineWidth(1.5);
}

//--------------------------------------------------------------
void ofApp::update()
{
    camera.dolly(-0.6);
    // camera.rollDeg(0.05);

    //  creating lines
    float static time;
    if (ofGetElapsedTimef() - time > ofRandom(.3, 1.7) && lines.size() < 30)
    {
        time = ofGetElapsedTimef();
        lines.push_back(ofPolyline());
        scales.push_back(1);
        alpha.push_back(150);
        scalar.push_back(ofRandom(.05, .2));
        matrix.push_back(ofMatrix4x4());
        color.push_back(ofColor(palette[rand() % 5], alpha.back()));
        c++;
        counter.push_back(c);

        // possible endpoints
        glm::vec3 endpoints[4] = {
            {ofRandomWidth(), h + 0, camera.getZ() + 300},  // BOTTOM
            {ofRandomWidth(), -0, camera.getZ() + 300},     // TOP
            {w + 0, ofRandomHeight(), camera.getZ() + 300}, // RIGHT
            {-0, ofRandomHeight(), camera.getZ() + 300}     // LEFT
        };

        int start_index = floor(ofRandom(0, 4));
        int end_index;
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
        glm::vec3 direction = glm::normalize(end[i] - emitter[i]);
        glm::vec3 normal = perp2D(direction);
        // glm::vec3 noise = normal * ofSignedNoise(counter[i] * 1000, ofGetElapsedTimef())*10;
        float length = 10;
        emitter[i] += direction * length; // + noise;

        matrix[i].glTranslate(w / 2, h / 2, 0);
        matrix[i].glRotate(scalar[i], 0, 0, 1);
        matrix[i].glTranslate(-w / 2, -h / 2, 0);

        if (end[i].z < camera.getZ() + 100)
        {
            alpha[i] -= 2;
            color[i] = (color[i], alpha[i]);
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
            color.erase(color.begin())
        }
    }

    showFps();

    // recorder.record("/home/ezequiel/Videos/flowfield", 18, 1);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    for (int j = 0; j < (int)lines.size(); j++)
    {
        camera.begin();
        ofMultMatrix(matrix[j]);
        ofSetColor(color[j]);
        // draw normals
        float width = 4;
        float density = .65 * lines[j].getPerimeter();
        for (int i = 0; i < density; i++)
        {
            float length = width - width * i / density; // decreasing
            glm::vec3 point = lines[j].getPointAtPercent(i / density);

            // avoid to draw outside screen objects
            glm::vec3 pt_screen = point; // camera.worldToScreen(point);
            glm::mat4 m = matrix[j];
            glm::rotate(m, -.02f * scalar[j], glm::vec3(0, 0, 1));
            glm::vec4 v = {pt_screen, 1.f};
            pt_screen = m * v;
            pt_screen = camera.worldToScreen(pt_screen);
            if (pt_screen.x > -50 && pt_screen.x < w + 50 &&
                pt_screen.y > -50 && pt_screen.y < h + 50)
            {
                float index = lines[j].getIndexAtPercent(i / density);
                glm::vec3 normal = (lines[j].getNormalAtIndexInterpolated(index));
                normal = glm::normalize(normal); // * length;
                point += normal * ofSignedNoise(i / 50.f, scalar[j] * 50, ofGetElapsedTimef() / 100.f) * 30;

                ofDrawCircle(point.x, point.y, point.z, length);
            }
        }
        camera.end();
    }
}

void ofApp::exit()
{
    // recorder.stopRecording();
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
