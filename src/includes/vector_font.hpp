#include <string>
#include <vector>

namespace Core
{
    class VectorFont
    {
    private:
        /* data */
        std::string fontData_ = R"(A
-0.25,0.5  0.0,-0.5   0.25,0.5
-0.125,0.0  0.125,0.0
EOC
            )";

        void configure_(const std::vector<std::string> &lines);

    public:
        VectorFont(/* args */) = default;
        ~VectorFont() = default;

        void initialize();
    };

} // namespace Core
