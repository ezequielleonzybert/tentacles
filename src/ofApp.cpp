#include "ofApp.h"
#include "mathz.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofEnableAlphaBlending();
    ofBackground(0);
    ofSetVerticalSync(false);
    ofSetFrameRate(1000);
    ofSetCircleResolution(20);
    camera.setPosition(w / 2, h / 2, -500);
    camera.lookAt({w / 2, h / 2, 0});

    ofNoFill();
    ofSetLineWidth(1.5);
}

//--------------------------------------------------------------
void ofApp::update()
{
    camera.dolly(-0.2);
    camera.rollDeg(0.05);

    //  creating lines
    float static time;
    if (ofGetElapsedTimef() - time > ofRandom(.5, 2) && lines.size() < 30)
    {
        time = ofGetElapsedTimef();
        lines.push_back(ofPolyline());
        scales.push_back(1);
        rotations.push_back(0);
        alphas.push_back(150);
        scalar.push_back(ofRandom(.3, .7));
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
        glm::vec3 noise = normal * ofSignedNoise(counter[i] * 1000, ofGetElapsedTimef());
        float length = 1;
        emitter[i] += direction * length + noise;
        rotations[i] += .05 * scalar[i];
        scales[i] += 0.0003;
        if (end[i].z < camera.getZ() + 100)
        {
            alphas[i] -= 1;
        }

        if (alphas[i] <= 0)
        {
            lines.erase(lines.begin());
            scales.erase(scales.begin());
            alphas.erase(alphas.begin());
            emitter.erase(emitter.begin());
            end.erase(end.begin());
            rotations.erase(rotations.begin());
            scalar.erase(scalar.begin());
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
        // draw normals
        float width = 7.5;
        float density = .65 * lines[j].getPerimeter();
        for (int i = 0; i < density; i++)
        {
            float length = width - width * i / density;
            glm::vec3 point = lines[j].getPointAtPercent(i / density);
            glm::vec3 pt_screen = camera.worldToScreen(point);
            if (pt_screen.x > -50 && pt_screen.x < w + 50)
            {
                float index = lines[j].getIndexAtPercent(i / density);
                glm::vec3 normal = (lines[j].getNormalAtIndexInterpolated(index));
                normal = glm::normalize(normal) * length;

                camera.begin();
                ofSetColor(ofColor::white, alphas[j]);
                ofDrawCircle(point.x, point.y, point.z, length);
                camera.end();
            }
        }
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
