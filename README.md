# Nội dung
- [Trò chơi con rắn](#Trò-chơi-con-rắn)
- [Cách sử dụng](#Cách-sử-dụng)
- [Thành viên](#Thành-viên)
- [Các file trong repo](#Các-file-trong-repo)
# Trò chơi con rắn
Trò chơi được ra mắt đầu tiên bởi hãng Gremlin (Mỹ) có tên Blockade vào năm 1976, và trở nên phổ biến khi được tích hợp trên Nokia 6110 bởi Taneli Armanto (Phần Lan) có cơ chế điều khiển con rắn ăn mồi, tránh đâm vào tường hoặc chính mình.<br>
Thông tin chi tiết về trò chơi: [Trò chơi con răn](https://en.wikipedia.org/wiki/Snake_(video_game_genre))<br>
Trong trò chơi này, con rắn sẽ được chơi với các map với kích cỡ bất kỳ

![image](https://www.multiplicationgames.com/images/blogs/multiplication-snake-screenshot.png)

# Cách sử dụng
Trước khi sử dụng tải thư viện `SFML 3.0.0` và `CMake 3.31.7` phù hợp với máy của bạn <br>
Tải dự án về và chạy trên CMake <br>
Trong giao diện CMake, vào mục Configure và chọn thư viện phù hợp (ưu tiên Visual Studio Build Tools 2022 Release) <br>
Vào mục Launch, ấn chọn main <br>
Khi vào giao diện có thể lựa chọn Training hoặc Chơi, đối với Training người sử dụng có thể thay đổi số lần train, kích thước map hoặc số bước tối đa kể từ khi ăn thức ăn cuối:
![Image](https://github.com/user-attachments/assets/ebcdbd0e-4fdc-452e-8f1d-c54192c2499f)
Đối với chế độ Chơi, người sử dụng có thể thay đổi kích cỡ của map, hoặc là có thể lựa chọn chế đồ người chơi hoặc AI chơi
![Image](https://github.com/user-attachments/assets/e52d74de-c422-41dc-b4ba-3ffd8d0afa7a)
Khi vào trò chơi, ta có thể lựa chọn:<br>
- Current Individual: Lựa chọn từng cá thể còn lại sau khi thực hiện train
- New Game: Thực hiện trò chơi mới
- Stop Game: Dừng lại trò chơi
![Image](https://github.com/user-attachments/assets/1229e104-0c4a-46f8-8afb-bb9b29143154)
# Thành viên
Là các sinh viên Khoa học máy tính - K68 Đại học Bách khoa Hà Nội, dưới sự hướng dẫn của PGS.TS Lê Thanh Hương:
- Trần Phương Nam 
- Nguyễn Ngọc Lê Minh
- Vũ Bình Minh
- Nguyễn Trung Hiếu
- Đào Kiều Thịnh Quang
# Các file trong repo
`Activation.hpp` : Chứa hàm kích hoạt trong mạng <br>
`BoardRenderer.hpp`: Vẽ mạng Neural <br>
`GUI.cpp`: Chứa các hàm thực hiện trò chơi <br>
`GUI.hpp`: File header chứa class GUI <br>
`GeneticAlgorithm.cpp`: Thuật toán di truyền <br>
`GeneticAlgorithm.hpp`: File header hỗ trợ thuật toán di truyền <br>
`NetworkRenderer.hpp`: Vẽ giao diện và thực hiện xếp layer cho mạng <br>
`Neural.cpp`: Xây dựng các phương thức liên kết của mạng <br>
`Neural.hpp`: Chứa các định nghĩa cơ bản của mạng Neural <br>
`Population.cpp`: Xây dựng quần thể <br>
`Population.hpp`: Header xây dựng cơ bản cho quần thể <br>
`RNG.hpp`: Chứa các phân phối xác suất cho mạng<br>
`Serializer.hpp`: Nơi để ghi các hàm lưu file<br>
`SnakeEngine.cpp`: Cách con rắn di chuyển<br>
`SnakeEngine.hpp`: Xây dựng cơ bản con rắn<br>
`config.hpp`: Chứa thông số di truyền<br>
`main.cpp`: Hàm chính<br>
