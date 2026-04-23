#ifndef ABSTRACT_HANDELER_HPP
#define ABSTRACT_HANDELER_HPP

class AbstractHandeler {
    public:
        virtual ~AbstractHandeler() = default;
        virtual void handle_client();
};

#endif