# PUBLIC / PRIVATE 单变量实验

本实验只改变 `FD_VISIBILITY_PUBLIC`。库自身始终能看到 `include/`；
消费者位于另一个目录，只在 `PUBLIC` 时继承该路径。

```bash
cmake -S . -B /tmp/d05-vis-public -DFD_VISIBILITY_PUBLIC=ON
cmake --build /tmp/d05-vis-public

cmake -S . -B /tmp/d05-vis-private -DFD_VISIBILITY_PUBLIC=OFF
cmake --build /tmp/d05-vis-private
```

第一组应成功。第二组应在编译 `app/main.cpp` 时得到
`'add.hpp' file not found`。失败发生在编译阶段，不是链接阶段。
