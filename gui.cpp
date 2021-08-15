#include "gui.hpp"

bool editorOpen = false;
bool lightsOpen = false;
bool infoOpen = true;
bool isquit = false;

bool showDemoWindow = false;

void initGui()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 330 core";
    IMGUI_CHECKVERSION();
    ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

// this function is code duplication: BAD!
ShaderProgramSources parseShader(const std::string &vertexpath, const std::string &fragmentpath)
{
    std::ifstream vertstream(vertexpath);
    std::ifstream fragstream(fragmentpath);

    std::string vert_line;
    std::string frag_line;

    std::stringstream vstream;
    std::stringstream fstream;

    while (std::getline(vertstream, vert_line))
    {
        vstream << vert_line << '\n';
    }
    while (std::getline(fragstream, frag_line))
    {
        fstream << frag_line << '\n';
    }
    
    std::string vert_str = vstream.str();
    std::string frag_str = fstream.str();

    //return CompileShader(vert_str.c_str(), frag_str.c_str());
    return {vert_str, frag_str};
}

void drawGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    
    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow();
    }

    // TOP MENU BAR
    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                std::cout << "new item dialog test" << std::endl;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit"))
            {
                isquit = true;
            }
            
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL + Z"))
            {
                std::cout << "undo item dialog test" << std::endl;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Shader Editor"))
            {
                editorOpen = true;
            }
            if (ImGui::MenuItem("Lights Editor"))
            {
                lightsOpen = true;
            }
            if (ImGui::MenuItem("Info Window"))
            {
                infoOpen = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("ImGUI Demo Window"))
            {
                if (showDemoWindow)
                {
                    showDemoWindow = false;
                }
                else
                {
                    showDemoWindow = true;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // SHADER EDITOR WINDOW
    if (editorOpen)
    {
        static char textBufVertex[1024 * 16], textBufFragment[1024 * 16];
        //parse strings of shader files into buffers
        static bool isParsed = false;
        if (isParsed == false)
        {
            strcpy(textBufVertex, parseShader("shaders/vertex.glsl", "shaders/fragment.glsl").VertexSource.c_str());
            strcpy(textBufFragment, parseShader("shaders/vertex.glsl", "shaders/fragment.glsl").FragmentSouce.c_str());
            isParsed = true;
        }
        ImGui::Begin("Shader Editor", &editorOpen);
        if (ImGui::BeginTabBar("##Tabs",ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Vertex Shader"))
            {
                ImGui::InputTextMultiline("##source", textBufVertex, IM_ARRAYSIZE(textBufVertex), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 24), ImGuiInputTextFlags_AllowTabInput);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Fragment Shader"))
            {
                ImGui::InputTextMultiline("##source", textBufFragment, IM_ARRAYSIZE(textBufFragment), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 24), ImGuiInputTextFlags_AllowTabInput);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        if(ImGui::Button("Save"))
        {
            std::string out_vertex, out_fragment;
            std::ofstream vf("shaders/temp_vertex.glsl"), ff("shaders/temp_fragment.glsl");
            vf << out_vertex.append(textBufVertex);
            ff << out_fragment.append(textBufFragment);
            vf.close();
            ff.close();
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply"))
        {
            shader.Unbind();
            shader.Build("shaders/temp_vertex.glsl", "shaders/temp_fragment.glsl");
            shader.Bind();
        }    
        
        ImGui::End();
    }

    // INFO & GENERALS WINDOW
    if (infoOpen)
    {
        ImGui::Begin("Info", &infoOpen);
        ImGui::ColorEdit3("background color", bgcol);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // QUIT CONFIRMATION WINDOW
    if (isquit)
    {
        ImGui::OpenPopup("Quit?");

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure?");
            ImGui::Separator();
            if (ImGui::Button("Yes"))
            {
                //exit(0);
                closemsg = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                ImGui::CloseCurrentPopup();
                isquit = false;
            }
            ImGui::EndPopup();
        }
    }

    //LIGHT SETTINGS WINDOW
    if(lightsOpen)
    {
        ImGui::Begin("Lights Editor", &lightsOpen);
        ImGui::Text("Point Light 01");
        ImGui::Checkbox("Point Light 01 Active", &pointLightActive);
        ImGui::ColorEdit3("Point Light Color", lightCol);
        ImGui::InputFloat3("Light Attenuation", lightAttenuation);
        ImGui::DragFloat3("Point Light Position", lightPos);
        ImGui::Checkbox("Point Light Orbit", &isOrbitLight);
        ImGui::End();
    }   
}

void guiRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
