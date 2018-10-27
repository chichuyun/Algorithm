!----------------------------------------------------------------------------!
! genetic algorithm
!----------------------------------------------------------------------------!
module main_fun
    implicit none

    private
    public :: genetic

    type genetic
        private
        integer, public :: population, iter_num, numerical
        integer, public :: seed
        integer :: length, retain_num
        real, public :: mutation_rate = 0.01
        real, public :: retain_rate = 0.2
        real, public :: random_rate = 0.5
        real, public :: upperb, lowerb
        real, allocatable :: fitness(:), var(:)
        real, allocatable :: trace(:,:)
        integer, allocatable :: dec_len(:)
        integer, allocatable :: Chrom(:,:)
        procedure(func_fit), public, nopass, pointer :: func => null()
    contains
        private
        procedure :: Initialization
        procedure :: Crossover
        procedure :: Mutation
        procedure :: Selection
        procedure :: get_length
        procedure :: sort => sort_fitness
        procedure :: bin2dec => binary2decimal
        procedure, public :: Evolve => Evolution
        procedure, public :: Output
        final :: destructor
    end type genetic

    interface
        real function func_fit(x)
            implicit none
            real :: x
        end function func_fit
    end interface
contains
!------------------------------------------! 获得染色体长度
    subroutine get_length(this)
        implicit none
        class(genetic) :: this
        this%length = ceiling(log((this%upperb-this%lowerb) &
                             *10**(this%numerical))/log(2.0))
    end subroutine get_length
!------------------------------------------! 二进制 => 十进制
    subroutine binary2decimal(this)
        implicit none
        class(genetic) :: this
        integer :: pop_i

        do pop_i=1,this%population
            this%var(pop_i) = float(dot_product(this%dec_len,this%Chrom(:,pop_i)))
        end do
        this%var = this%lowerb + this%var*(this%upperb-this%lowerb)/(2.**this%length-1)
    end subroutine binary2decimal
!------------------------------------------! 适应度排序
    subroutine sort_fitness(this)
        implicit none
        class(genetic) :: this
        integer :: temp_chrom(this%length), pop_i, pop_j
        real :: temp_fit, temp_var
        call this%bin2dec()
        do pop_i=1,this%population
            this%fitness(pop_i) = this%func(this%var(pop_i))
        end do

        do pop_i=1,this%population-1
            do pop_j=1,this%population-1
                if(this%fitness(pop_j)<this%fitness(pop_j+1)) then
                    temp_var = this%var(pop_j)
                    this%var(pop_j) = this%var(pop_j+1)
                    this%var(pop_j+1) = temp_var

                    temp_fit = this%fitness(pop_j)
                    this%fitness(pop_j) = this%fitness(pop_j+1)
                    this%fitness(pop_j+1) = temp_fit

                    temp_chrom = this%Chrom(:,pop_j+1)
                    this%Chrom(:,pop_j) = this%Chrom(:,pop_j+1)
                    this%Chrom(:,pop_j+1) = temp_chrom
                end if
            end do
        end do
    end subroutine sort_fitness
!------------------------------------------! 初始化
    subroutine Initialization(this)
        implicit none
        integer :: i, pop_i
        real :: first_random
        class(genetic) :: this
        call get_length(this)
        allocate(this%Chrom(this%length,this%population))
        allocate(this%fitness(this%population))
        allocate(this%var(this%population))
        allocate(this%dec_len(this%length))
        allocate(this%trace(2,this%iter_num))
        this%trace = 0.0
        forall(i=1:this%length) this%dec_len(i)=2**(i-1)

        first_random = rand(this%seed)
        do pop_i=1,this%population
            do i=1,this%length
                if(ran()>0.5) then
                    this%Chrom(i,pop_i) = 0
                else
                    this%Chrom(i,pop_i) = 1
                end if
            end do
        end do
    end subroutine Initialization
!------------------------------------------! 演化
    subroutine Evolution(this)
        implicit none
        class(genetic) :: this
        integer :: i, max_i(1)

        call this%Initialization()  ! 初始化
        do i=1,this%iter_num
            call this%Selection()   ! 选择
            call this%Crossover()   ! 交叉
            call this%Mutation()    ! 变异
            max_i = maxloc(this%fitness)
            this%trace(1,i) = this%var(max_i(1))
            this%trace(2,i) = this%fitness(max_i(1))
        end do
    end subroutine Evolution
!------------------------------------------! 选择
    subroutine Selection(this)
        implicit none

        class(genetic) :: this
        integer :: pop_i, pop_j, num
        integer, dimension(:,:), allocatable :: newChrom
        allocate(newChrom(this%length,this%population))

        call this%sort()
        this%retain_num = ceiling(this%population*this%retain_rate)
!        newChrom = 0
        newChrom(:,1:this%retain_num) = this%Chrom(:,1:this%retain_num)
        pop_j = this%retain_num+1
        num = 0
        do pop_i=this%retain_num+1,this%population
            if(this%random_rate>ran()) then
                newChrom(:,pop_j) = this%Chrom(:,pop_i)
                pop_j = pop_j + 1
                num = num + 1
            end if
        end do
        this%Chrom = newChrom
        this%retain_num = this%retain_num + num
        deallocate(newChrom)
    end subroutine Selection
!------------------------------------------! 交叉
    subroutine Crossover(this)
        implicit none
        class(genetic) :: this
        integer :: pop_i, j
        integer, allocatable :: male(:), female(:), child(:)
        allocate(male(this%length),female(this%length),child(this%length))
        do pop_i=this%retain_num+1,this%population
            male = this%Chrom(:,ceiling(this%retain_num*ran()))
            female = this%Chrom(:,ceiling(this%retain_num*ran()))
            do j=1,this%length
                if(ran()>0.5) then
                    child(j) = male(j)
                else
                    child(j) = female(j)
                end if
            end do
            this%Chrom(:,pop_i) = child
        end do
        deallocate(male,female,child)
    end subroutine Crossover
!------------------------------------------! 变异
    subroutine Mutation(this)
        implicit none
        class(genetic) :: this
        integer :: pop_i, i
        do pop_i=this%retain_num+1,this%population
            if(this%mutation_rate>ran()) then
                i = ceiling(this%length*ran())
                this%Chrom(i,pop_i) = 1 - this%Chrom(i,pop_i)  ! 1=>0, 0=>1
            end if
        end do
    end subroutine Mutation
!------------------------------------------! 输出
    subroutine Output(this)
        implicit none
        class(genetic) :: this
        integer :: i
        open(10, file = 'trace.txt')
        do i=1,this%iter_num
            write(10,*) i, this%trace(:,i)
            write(*,*)  i, this%trace(:,i)
        end do
    end subroutine Output
    subroutine destructor(this)
        implicit none
        type(genetic) :: this
        deallocate(this%Chrom, this%fitness)
        deallocate(this%var, this%dec_len, this%trace)
    end subroutine destructor
end module
!----------------------------------------------------------------------------!
real function Fitness_func(x) ! sufficiency function
    implicit none
    real :: x
    Fitness_func = x + 10*sin(5*x) + 7*cos(4*x)
end function Fitness_func
!----------------------------------------------------------------------------!
program main
    use main_fun
    implicit none
    real, external :: Fitness_func

    type(genetic) :: process

    process%func => Fitness_func  ! 适应度函数 vector
    process%seed = 123456         ! 随机数种子
    process%iter_num = 100        ! 迭代次数
    process%population = 100      ! 种群大小
    process%numerical = 3         ! 有效数字
    process%retain_rate = 0.2     ! 存活率
    process%random_rate = 0.5     ! 随机率
    process%mutation_rate = 0.01  ! 变异率
    process%upperb = 9.0          ! 参数上限 vector
    process%lowerb = 0.0          ! 参数下限 vector

    call process%Evolve()
    call process%Output()

    stop
end program main
!----------------------------------------------------------------------------!
