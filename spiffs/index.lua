print("Hello Luna!")

--初始化GPIO
--  GPIO端口[0, 40)
--  模式(假=输出，真=输入)
--  上拉电阻(假=关闭，真=打开)
--  下拉电阻(假=关闭，真=打开)
gpio_init(1, false, true, true)

--设置GPIO电平
--  GPIO端口 
--  电平(假=0，真=1)
gpio_set_level(1, true)

--等待
--  毫秒数
sleep(500)

for i=1,4 do
  print(i)
  gpio_set_level(1, i&1==0)
  sleep(100)
end

