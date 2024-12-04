#pragma once
#define __SNAKE__
#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <utility>
#include "observer.hpp"
#include "db.hpp"

#define SNAKE_HEAD  1
#define SNAKE_BODY  2
#define APPLE   -1

#define WIDTH 10
#define HEIGHT 20

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;


void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();


template <typename _T1, typename _T2, typename = std::enable_if<std::is_integral<_T1>::value && std::is_integral<_T2>::value > >
std::pair<_T1,_T2> operator-(const std::pair<_T1,_T2> &__lhs, const std::pair<_T1, _T2> &__rhs){
    return std::pair<_T1, _T2>(__lhs.first - __rhs.first, __lhs.second - __rhs.second);
};

template <typename _T1, typename _T2, typename = std::enable_if<std::is_integral<_T1>::value && std::is_integral<_T2>::value > >
std::pair<_T1,_T2> operator+(const std::pair<_T1,_T2> &__lhs, const std::pair<_T1, _T2> &__rhs){
    return std::pair<_T1, _T2>(__lhs.first + __rhs.first, __lhs.second + __rhs.second);
};

template <typename _T, class _Allocator = std::allocator<_T>>
struct __field{
public:
    typedef _Allocator allocator_type;
    typedef typename allocator_type::value_type value_type;
    typedef value_type** pointer;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    
    typedef typename std::allocator_traits<allocator_type>::template rebind_alloc<value_type*> pointer_allocator;
    typedef std::allocator_traits<pointer_allocator> pointer_alloc_type;

    typedef typename std::allocator_traits<allocator_type>::template rebind_alloc<value_type> value_allocator;
    typedef std::allocator_traits<value_allocator> value_alloc_type;

    
protected:
    pointer data_ = nullptr;

    pointer_allocator pointer_alloc_;
    value_allocator value_alloc_;

    size_type rows_;
    size_type cols_;

public:
    __field(size_type __rows, size_type __cols):rows_(__rows), cols_(__cols){
        this->data_ = std::allocator_traits<pointer_allocator>::allocate(this->pointer_alloc_, __rows);
        for(size_type __i = 0; __i < __rows; __i++){
            this->data_[__i] = std::allocator_traits<value_allocator>::allocate(this->value_alloc_, __cols);
            for(size_type __j = 0; __j < __cols; __j++){
                std::allocator_traits<value_allocator>::construct(this->value_alloc_, this->data_[__i] + __j, value_type());
            }
        }
    }

    __field(const __field& __f){
        this->__copy(__f);
    }

    __field(__field&& __f){
        this->__move(std::move(__f));
    }

    ~__field(){
        destroy();
    }

    __field& operator=(const __field& __f){
        if(this != &__f){
            this->__copy(__f);
        }

        return *this;
    }

    __field& operator=(__field&& __f){
        if(this != &__f){
            this->__move(std::move(__f));
        }   

        return *this;
    }

    void destroy(){
        if(this->data_ != nullptr){
            for(size_type __i = 0; __i < this->rows_; __i++){
                for(size_type __j = 0; __j < this->cols_; __j++){
                    std::allocator_traits<value_allocator>::destroy(this->value_alloc_, this->data_[__i] + __j);
                }
                std::allocator_traits<value_allocator>::deallocate(this->value_alloc_, this->data_[__i], this->cols_);
            }
            std::allocator_traits<pointer_allocator>::deallocate(this->pointer_alloc_, this->data_, this->rows_);
            this->data_ = nullptr;
        }
    }

    void fill(const_reference __ref){
        if(this->data_ != nullptr){
            for(size_type __i = 0; __i < this->rows_; __i++){
                std::fill(this->data_[__i], this->data_[__i] + this->cols_, __ref);
            }
        }
    }

    size_type rows() const{
        return this->rows_;
    }

    size_type cols() const{
        return this->cols_;
    }

    inline pointer data() const{
        return this->data_;
    }

    reference operator[](size_type __n){
        if(__n >= this->rows_){
            throw std::out_of_range("operator[] out ouf bounds");
        }
        return this->data_[__n];
    }

    reference at(size_type __row, size_type __col){
        return this->data_[__row][__col];
    }

private:
    void __move(__field &&__f){
        this->data_ = __f.data_;
        this->rows_ = __f.rows_;
        this->cols_ = __f.cols_;
        this->value_alloc_ = std::move(__f.value_alloc_);
        this->pointer_alloc_ = std::move(__f.pointer_alloc_);

        __f.data_ = nullptr;
        __f.rows_ = __f.cols_ = 0;
    }

    void __copy(const __field &__f){
        this->data_ = std::allocator_traits<pointer_allocator>::allocate(this->pointer_alloc_, __f.rows_);
        for(size_type __i = 0; __i < __f.rows_; __i++){
            this->data_[__i] = std::allocator_traits<value_allocator>::allocate(this->value_alloc_, __f.cols_);
            for(size_type __j = 0; __j < __f.cols_; __j++){
                std::allocator_traits<value_allocator>::construct(this->value_alloc_, this->data_[__i] + __j, __f.data_[__i][__j]);
            }
        }
    }
};



struct __stats {
    __field<int> field_;
    __field<int> next_;
    int score_ = 0;
    int h_score_ = __db::__instance()->load_scores();
    int level_ = 1;
    int speed_ = 1;
    int pause_ = 0;

public:
    __stats() : field_(HEIGHT, WIDTH), next_(HEIGHT, WIDTH) {}
    ~__stats(){
        this->field_.destroy();
        this->next_.destroy();
    }

    void increase_score(){
        this->score_++;

        if(this->score_ > this->h_score_){
            __db::__instance()->store_scores(this->score_);
            this->h_score_ = this->score_;
        }

        if(this->score_ != 0 & this->score_ % 2 == 0 && this->level_ <= 10){
            this->level_++;
            this->speed_ *= 2;
        }
    }

    inline __field<int>& field() { return this->field_; }
    inline const __field<int>& field() const { return this->field_; }
    
    inline __field<int>& next() { return this->next_; }
    inline const __field<int>& next() const { return this->next_; }

    inline int& score() { return this->score_; }
    inline const int& score() const { return this->score_; }

    inline int& level() { return this->level_; }
    inline const int& level() const { return this->level_; }

    inline int& high_score() { return this->h_score_; }
    inline const int& high_score() const { return this->h_score_; }

    inline int& speed() { return this->speed_; }
    inline const int& speed() const { return this->speed_; }

    inline int& pause() { return this->pause_; }
    inline const int& pause() const { return this->pause_; }
};


class __timer{
    std::chrono::steady_clock::time_point __tm = std::chrono::steady_clock::now();

public:
    __timer(){

    };
    
    long long ellapsed_time(){
        return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - this->__tm ).count();
    }

    void refresh(){
        this->__tm = std::chrono::steady_clock::now();
    }
};

class __snake{
    std::vector<std::pair<int,int>> data_ = {
        {0,0},{0,1},{0,2}
    };
    std::pair<int,int> velocity_ = {1,0};

public:
    __snake(const std::pair<int,int>& __pos = std::pair<int, int>(0,0)){
        this->data_.front() = __pos;
        for(auto it = std::next(this->data_.begin()) ; it != this->data_.end(); it++){
            *it = __pos + std::pair<int,int>(0, std::distance(this->data_.begin(), it) );
        }
    }

    const std::vector<std::pair<int,int>>& data() const{
        return this->data_;
    }
    
    const std::pair<int,int>& velocity() const{
        return this->velocity_;
    }

    void move(){
        for(std::size_t __i = this->data_.size() - 1; __i > 0; __i--){
            this->data_.at(__i) = this->data_.at(__i - 1);
        }
        this->data_.front() = this->data_.front() + this->velocity_;
    }

    void set_velocity(const std::pair<int,int> &__velocity){
        if(this->data_.at(1) == this->data_.front() + __velocity) return;
        this->velocity_ = __velocity;
    }

    void increase(){
        std::pair<int,int> __diff = this->data_.back() - this->data_.at(this->data_.size() - 2);
        std::pair<int,int> __next = this->data_.back() - __diff;
        this->data_.emplace_back(__next);
    }

    bool decrease(){
        if(this->data_.empty()) return false;
        this->data_.pop_back();
        return true;
    }
};

class __engine{
private:
    __field<int> field_;
    __stats stats_;
    __timer timer_;
    __snake snake_;
    std::pair<int, int> apple_ = {0,0};
    std::pair<int, int> velocity_ = {1,0};

    __engine():field_(HEIGHT, WIDTH), snake_(std::pair<int,int>(HEIGHT / 2, WIDTH / 2)){

    }

    void __refresh_state(){
        this->stats_ = __stats();
        this->snake_ = __snake(std::pair<int,int>(HEIGHT / 2, WIDTH / 2));
        this->timer_ = __timer();
        this->field_ = __field<int>(HEIGHT, WIDTH);
        __rand_apple();
    }

    bool __check_collision(){
        std::pair<int,int> __pos = this->snake_.data().front() + this->snake_.velocity();
        if(__pos.first >= static_cast<int>(this->field_.rows()) || __pos.first < 0 || __pos.second < 0 || __pos.second >= static_cast<int>(this->field_.cols())){
            return true;
        }

        std::vector<std::pair<int,int>>::const_iterator it = std::find(this->snake_.data().begin(), this->snake_.data().end(), __pos);
        if(it != this->snake_.data().end()){
            return true;
        }
        
        return false;
    }

    void __check_apple(){
        if(std::find(this->snake_.data().begin(), this->snake_.data().end(), this->apple_) != this->snake_.data().end()){
            this->snake_.increase();
            this->__rand_apple();
            this->stats_.increase_score();
        }
    }

    void __rand_apple(){
        while( std::find(this->snake_.data().begin(), this->snake_.data().end(), this->apple_) != this->snake_.data().end()) 
            this->apple_ = std::pair<int, int>(std::rand() % this->field_.rows(), std::rand() % this->field_.cols());
    }

    bool __do_update(){
        return (double)timer_.ellapsed_time() >= 100.0 / (1 + 0.1 * stats_.speed());
    }

public:
    __observer<> finish; 

    ~__engine(){

    }

    GameInfo_t data(){
        for(std::size_t __i = 0; __i < this->field_.rows(); __i++){
            for(std::size_t __j = 0; __j < this->field_.cols(); __j++){
                this->field_.at(__i, __j) = this->stats_.field().at(__i, __j);
            }
        }
        
        for(const std::pair<int,int> &__pos : this->snake_.data()){
            this->field_.at(__pos.first, __pos.second) = SNAKE_BODY;
        }
        this->field_.at(this->snake_.data().front().first, this->snake_.data().front().second) = SNAKE_HEAD;

        this->field_.at(this->apple_.first, this->apple_.second) = APPLE;

        GameInfo_t __dt = {
            .field              = reinterpret_cast<int**>(this->field_.data()),
            .next               =   reinterpret_cast<int**>(this->stats_.next().data()),
            .level              = this->stats_.level(),
            .high_score         = this->stats_.high_score(),
            .pause              = this->stats_.pause(),
            .score              = this->stats_.score(),
            .speed              = this->stats_.speed(),
        };
        return __dt;
    }

    void user_input(UserAction_t __key, __attribute__((unused)) bool hold){
        if(__key == UserAction_t::Left){
            this->snake_.set_velocity({0, -1});
        }else if(__key == UserAction_t::Right){
            this->snake_.set_velocity({0, 1});
        }else if(__key == UserAction_t::Up){
            this->snake_.set_velocity({-1, 0});
        }else if(__key == UserAction_t::Down){
            this->snake_.set_velocity({1, 0});
        }
    }


    void update(){
        if(__do_update()){
            if(this->__check_collision()){
                this->finish.notify();
            }else{
                this->snake_.move();
                this->__check_apple();
            }
            this->timer_.refresh();
        }
    }

    void start(){
        this->__refresh_state();
    }

    static std::shared_ptr<__engine> __instance();
};
