! 感谢网友@尼古拉斯·魏 对代码的改进
!----------------------------------------------------------------------------!
! genetic algorithm
!----------------------------------------------------------------------------!
module GA
    implicit none
    
    private
    public :: genetic, IB, I1B, I2B, RP
    !一般IB=4时,最大编码长度为31=4*8-1 最左边符号位不用 确保正数
    integer(kind=selected_int_kind(1)), parameter :: IB  = selected_int_kind(9) ! @尼古拉斯·魏
    integer(kind=selected_int_kind(1)), parameter :: I1B = selected_int_kind(1) ! @尼古拉斯·魏
    integer(kind=selected_int_kind(1)), parameter :: I2B = selected_int_kind(3) ! @尼古拉斯·魏
    integer(kind=selected_int_kind(1)), parameter :: RP  = selected_real_kind(6) !控制单、双精度 @尼古拉斯·魏
    
    type genetic
        private
        integer, public :: seed                            ! 随机数种子
        integer(kind=I2B), public :: population, iter_num  ! 种群大小、迭代次数、
        integer(kind=I1B) :: length                        ! 染色体长度
        integer(kind=I2B) :: retain_num                    ! 存活种群数
        real(kind=RP), public :: mutation_rate = 0.01_RP   ! 变异率
        real(kind=RP), public :: retain_rate = 0.2_RP      ! 存活率
        real(kind=RP), public :: random_rate = 0.5_RP      ! 随机率
        real(kind=RP), allocatable, public :: upperb(:), lowerb(:)  ! 参数上限、下限
        real(kind=RP), allocatable :: fitness(:), var(:,:) ! fitness为适应度即函数值,var为十进制种群自变量
        real(kind=RP), allocatable :: trace(:,:)           ! 历次迭代对应的自变量、适应度即函数值
        integer(kind=IB), allocatable :: Chrom(:,:)        ! 种群二进制编码,必须为正数
        ! 过程指针是类字段变量不是类过程  必须位于contains之前
        procedure(func_fit), public, nopass, pointer :: func => null() 
        logical, public :: IsMin  = .false.  ! .true.代表求最小值 .false. 代表求最大值(默认)   @尼古拉斯·魏
        logical, public :: IsGray = .false.  ! .true.代表格雷编码 .false. 代表二进制编码(默认) @尼古拉斯·魏
        integer(kind=I1B), public :: numerical, x_num = 1  ! 有效数字、函数自变量个数(默认为1)
    contains
        private
        procedure :: Initialization               ! 初始化
        procedure :: Crossover                    ! 交叉
        procedure :: Mutation                     ! 变异
        procedure :: Selection                    ! 选择
        procedure :: get_length                   ! 获得染色体长度
        procedure :: sort => sort_fitness         ! 适应度排序
        procedure :: bin2dec => binary2decimal    ! 二进制 => 十进制
        procedure, public :: Evolve => Evolution  ! 演化
        procedure, public :: Output               ! 输出
        procedure, public, nopass :: gray2binary  ! 编码函数 @尼古拉斯·魏
        procedure, public, nopass :: binary2gray  ! 解码函数 @尼古拉斯·魏 
        final :: destructor                       ! 析构函数
    end type genetic
    
    interface ! 显式接口,过程指针func对任何与接口func_fit一样的函数都可用
        real(kind=RP) function func_fit(x)
            import
            real(kind=RP), intent(in) :: x(:)
        end function func_fit
    end interface
contains
!------------------------------------------! 获得染色体长度
    subroutine get_length(this)
        implicit none
        class(genetic) :: this
        integer(kind=IB) :: a
        
        this%length = ceiling(log((MaxVal(this%upperb-this%lowerb)) &
                             *10**(this%numerical))/log(2.0_RP)) ! 小数部分进一
        
        write(*,*) "染色体长度:", this%length     ! @尼古拉斯·魏
        
        if (this%length > bit_size(a) - 1) then   ! @尼古拉斯·魏
            write(*,*) "预留的编码长度不足,请增大IB值！"
            stop ! 提前结束程序
        end if
    end subroutine get_length
!------------------------------------------! 二进制 => 十进制
    subroutine binary2decimal(this) ! 二进制 => 十进制
        implicit none
        
        class(genetic) :: this
        integer(kind=IB) :: C ! @尼古拉斯·魏
            integer(kind=I2B) :: pop_i
            integer(kind=I1B) :: pop_j
        
        C = 2**this%length-1 ! 常数 避免重复计算
        if(this%IsGray) call gray2binary(this%Chrom,this%Chrom) !格雷码转换为二进制编码
        
        do pop_i=1, this%population
            do pop_j=1, this%x_num ! @尼古拉斯·魏
                this%var(pop_j,pop_i) = this%lowerb(pop_j) + &
                this%Chrom(pop_j,pop_i)*(this%upperb(pop_j)-this%lowerb(pop_j))/C
            end do ! @尼古拉斯·魏
        end do
        
        if(this%IsGray) call binary2gray(this%Chrom,this%Chrom) ! 二进制编码转换为格雷码
    end subroutine binary2decimal
!------------------------------------------! 二进制编码转换为格雷码 @尼古拉斯·魏
    pure elemental subroutine binary2gray(b,g)
        implicit none
   
        integer(kind=IB), intent(in) :: b
        integer(kind=IB), intent(out) :: g
        ! ieor位异或  ishft位右移(负数代表右移)
        ! g = ieor(b,ishft(b,-1)) ! 参考c语言x^(x>>1) F95
        g = ieor(b,shiftr(b,1))   ! 参考c语言x^(x>>1) F08
    end subroutine binary2gray
!------------------------------------------! 格雷码转换为二进制编码 @尼古拉斯·魏
    pure elemental subroutine gray2binary(g,b)
        implicit none
   
        integer(kind=IB), intent(in) :: g
        integer(kind=IB), intent(out) :: b
        integer(kind=I1B) :: i,l
   
        l=bit_size(g) !内存总位数
        call mvbits(g,l-1,1,b,l-1)   !将g右边第l位信息复制到b相应位置
   
        do i=l-2,0,-1
            call mvbits(ieor(ibits(b,i+1,1),ibits(g,i,1)),0,1,b,i) !参考网上代码
        end do
    end subroutine gray2binary
!------------------------------------------! 适应度排序
    subroutine sort_fitness(this)
        implicit none
    
        class(genetic) :: this
        integer(kind=I2B) :: pop_i, pop_j
        integer(kind=IB)  :: temp_chrom(this%x_num)
        real(kind=RP)     :: temp_fit, temp_var(this%x_num)
    
        call this%bin2dec() ! 二进制 => 十进制
    
        do pop_i=1,this%population
                this%fitness(pop_i) = this%func(this%var(:,pop_i)) ! 种群适应度 即函数值
        end do
            ! 求最小值时需将适应度设成相反数
            if(this%IsMin) this%fitness=-this%fitness !增加
            ! 以下为冒泡排序法 还可继续优化
        do pop_i=this%population-1,1,-1 ! 优化后
            do pop_j=1,pop_i ! 优化后
                if(this%fitness(pop_j)<this%fitness(pop_j+1)) then ! 降序排序
                    ! 十进制自变量相应交换
                    temp_var(:) = this%var(:,pop_j)
                    this%var(:,pop_j) = this%var(:,pop_j+1)
                    this%var(:,pop_j+1) = temp_var(:)
                    ! 适应度相应交换
                    temp_fit = this%fitness(pop_j)
                    this%fitness(pop_j) = this%fitness(pop_j+1)
                    this%fitness(pop_j+1) = temp_fit
                    ! 二进制自变量相应交换
                    temp_chrom(:) = this%Chrom(:,pop_j+1)
                    this%Chrom(:,pop_j) = this%Chrom(:,pop_j+1)
                    this%Chrom(:,pop_j+1) = temp_chrom(:)
                end if
            end do
        end do
    end subroutine sort_fitness
!------------------------------------------! 初始化
    subroutine Initialization(this)
        implicit none
    
        integer(kind=I2B) :: pop_i
        integer(kind=I1B) :: i, pop_j
        real(kind=RP) :: first_random
        class(genetic) :: this
    
        call get_length(this) ! 获得染色体长度
        allocate(this%Chrom(this%x_num,this%population))
        allocate(this%fitness(this%population))
        allocate(this%var(this%x_num,this%population))
        allocate(this%trace(this%x_num+1,this%iter_num))
        this%Chrom=0 !一定要设为0 以保证符号位为0确保正数 增加
        this%trace = 0.0_RP
    
        first_random = rand(this%seed)
    
        do pop_i=1,this%population ! 种群二进制编码 一半概率为0 一半概率为1
            do pop_j=1,this%x_num ! 增加
                do i=1,this%length
                    if(rand()>0.5_RP) then
                    !注意ibclr(i,pos) 位置pos范围为0到bt_size(i)-1  0代表右边第1位  bt_size(i)-1为左边第1位
                        this%Chrom(pop_j,pop_i) = ibclr(this%Chrom(pop_j,pop_i),i-1) !在右边第i位设为0
                    else
                    !注意ibset(i,pos) 位置pos范围为0到bt_size(i)-1  0代表右边第1位  bt_size(i)-1为左边第1位
                        this%Chrom(pop_j,pop_i) = ibset(this%Chrom(pop_j,pop_i),i-1) !在右边第i位设为1
                    end if
                end do
            end do ! @尼古拉斯·魏
        end do
    end subroutine Initialization
!------------------------------------------! 演化
  subroutine Evolution(this)
    implicit none

    class(genetic) :: this
    integer(kind=I2B) :: i, max_i(1) ! 注意maxloc函数返回值max_i必须为一维数组 不能为变量

    call this%Initialization()  ! 初始化

    do i=1,this%iter_num ! 历次迭代
        call this%Selection()   ! 选择
        call this%Crossover()   ! 交叉
        call this%Mutation()    ! 变异
        max_i = maxloc(this%fitness) ! 适应度最大值对应的种群位置
        this%trace(1:this%x_num,i) = this%var(:,max_i(1))   ! 适应度最大值对应的自变量
        this%trace(this%x_num+1,i) = this%fitness(max_i(1)) ! 适应度最大值
    end do
    ! 求最小值时需将适应度恢复为正常值
    if(this%IsMin) this%trace(this%x_num+1,:) = -this%trace(this%x_num+1,:)
  end subroutine Evolution
!------------------------------------------! 选择
    subroutine Selection(this) ! 优胜劣汰
        implicit none
        
        class(genetic) :: this
        integer(kind=I2B) :: pop_i, pop_j, num
        integer(kind=IB), dimension(:,:), allocatable :: newChrom ! 种群二进制新的编码
        
        allocate(newChrom(this%x_num,this%population))
        
        call this%sort() ! 适应度排序 降序
        this%retain_num = ceiling(this%population*this%retain_rate) ! 存活种群数 小数部分进一
        newChrom = 0 ! 初值为0
        newChrom(:,1:this%retain_num) = this%Chrom(:,1:this%retain_num) ! 种群适度度排名前retain_num名才能存活
        pop_j = this%retain_num+1 ! 淘汰的第一个位置
        num = 0 ! 随机存活种群数
        
        do pop_i=this%retain_num+1,this%population  ! 排在retain_num名之后的种群随机存活
            if(this%random_rate>rand()) then ! 随机存活
                newChrom(:,pop_j) = this%Chrom(:,pop_i)
                pop_j = pop_j + 1
                num = num + 1
            end if
        end do
        
        this%Chrom = newChrom
        this%retain_num = this%retain_num + num ! 最终存活种群数 this%Chrom(this%retain_num+1:)=0
        deallocate(newChrom)
    end subroutine Selection
!------------------------------------------! 交叉
    subroutine Crossover(this)
        implicit none
        class(genetic) :: this
        integer(kind=I2B) :: pop_i
        integer(kind=I1B) :: j, pop_j
        integer(kind=IB) :: male, female, child

        do pop_i=this%retain_num+1,this%population ! 没有存活的空位
            do pop_j=1,this%x_num ! @尼古拉斯·魏
                male = this%Chrom(pop_j,ceiling(this%retain_num*rand()))   ! 存活种群中随机选出父种群
                female = this%Chrom(pop_j,ceiling(this%retain_num*rand())) ! 存活种群中随机选出母种群

                do j=1,this%length ! 子种群的二进制编码的每个位置
                    if(rand()>0.5_RP) then ! 一半概率继承父种群 一半概率继承母种群
                        call mvbits(male,j-1,1,child,j-1)   !将male右边第j位信息复制到child相应位置
                    else
                        call mvbits(female,j-1,1,child,j-1) !将female右边第j位信息复制到child相应位置
                    end if
                end do
                !将child所有信息复制到Chrom
                call mvbits(child,0,this%length,this%Chrom(pop_j,pop_i),0)
            end do ! @尼古拉斯·魏
        end do
    end subroutine Crossover
!------------------------------------------! 变异
    subroutine Mutation(this)
        implicit none

        class(genetic) :: this
        integer(kind=I2B) :: pop_i
        integer(kind=I1B) :: i, pop_j

        do pop_i=this%retain_num+1,this%population ! 子种群
            do pop_j=1,this%x_num ! 增加
                if(this%mutation_rate>rand()) then    ! mutation_rate为变异率
                    i = ceiling(this%length*rand())   ! 变异位置随机分布
                    ! 指定某一位取反 参考c语言x^=(1<<y) F95
                    ! ieor位异或  ishft位左移(正数代表左移)  实现指定某一位0变1 1变0
                    ! this%Chrom(pop_j,pop_i) = ieor(this%Chrom(pop_j,pop_i),ishft(1_IB,i-1))
                    ! 指定某一位取反 参考c语言x^=(1<<y) F08
                    this%Chrom(pop_j,pop_i) = ieor(this%Chrom(pop_j,pop_i),shiftl(1_IB,i-1)) 
                end if
            end do ! @尼古拉斯·魏
        end do
    end subroutine Mutation
!------------------------------------------! 输出
    subroutine Output(this)
        implicit none
   
        class(genetic) :: this
        integer(kind=I2B) :: i
   
        open(10, file = 'trace.txt') ! 打印历次迭代对应的自变量、适应度即函数值
          
        do i=1,this%iter_num
            write(10,*) i, this%trace(:,i)
            write(*,*)  i, this%trace(:,i)
        end do
    end subroutine Output
!------------------------------------------! 释放所有数组内存
    subroutine destructor(this)
        implicit none
        type(genetic) :: this
        
        deallocate(this%Chrom, this%fitness)
        deallocate(this%var, this%trace)
        deallocate(this%upperb, this%lowerb) !  @尼古拉斯·魏
    end subroutine destructor
end module GA
!----------------------------------------------------------------------------!
program main
    use GA
    implicit none
    type(genetic) :: process
    
    process%func => Fitness_func      ! 适应度函数 vector
    process%seed = 623456             ! 随机数种子
    process%iter_num = 200            ! 迭代次数
    process%population = 200          ! 种群大小
    process%numerical = 7             ! 有效数字
    process%retain_rate = 0.35_RP     ! 存活率
    process%random_rate = 0.35_RP     ! 随机率
    process%mutation_rate = 0.005_RP  ! 变异率
    process%x_num = 2                 ! 自变量个数
    !process%IsGray = .true.          ! .true.代表格雷编码
    !process%IsMin  = .true.          ! .true.代表求最小值
   
    process%upperb = [12.1_RP, 5.8_RP] ! 自变量上限
    process%lowerb = [-3.0_RP, 4.1_RP] ! 自变量下限
   
    call process%Evolve()         ! 演化
    call process%Output()         ! 输出
contains
!----------------------------------------------------------------------------!
    real(kind=RP) function Fitness_func(x) ! sufficiency function 适应度函数
        implicit none

        real(kind=RP), parameter :: PI=3.1415926535897_RP
        real(kind=RP), intent(in) :: x(:)

        !算例 -3.0<x(1)<12.1 4.1<x(2)<5.8  在(11.6255,5.7250)处有一个全局最大值38.8503
        Fitness_func = 21.5_RP + x(1) * Sin(4.0_RP * PI * x(1)) + x(2) * Sin(20.0_RP * PI * x(2))
    end function Fitness_func
!----------------------------------------------------------------------------!
end program main
!----------------------------------------------------------------------------!
!F2008
!The following bit intrinsics:
!LEADZ and TRAILZ for counting the number of leading and trailing zero bits,
!POPCNT and POPPAR for counting the number of one bits and returning the parity;
!BGE, BGT, BLE, and BLT for bitwise comparisons;
!DSHIFTL and DSHIFTR for combined left and right shifts,
!MASKL and MASKR for simple left and right justified masks,
!MERGE_BITS for a bitwise merge using a mask,
!SHIFTA, SHIFTL and SHIFTR for shift operations,
!and the transformational bit intrinsics IALL, IANY and IPARITY.
