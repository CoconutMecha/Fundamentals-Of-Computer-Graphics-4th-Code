#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Texture
{
    public:
        char* image_url;
        Texture(char* image_url)
        {
            this->image_url = image_url;
        }

    private:
        glm::vec4 compute_uv_coordinate(glm::vec4 coordinates_3,glm::mat4 mat_P)
        {
            return glm::vec4(1,1,1,1);
        }



};