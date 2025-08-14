
# University Teaching Management System (UTMS)


A comprehensive university management system with social features, implemented in C++ with a web interface using HTML/CSS and Makefile for build automation.

## 📝 Description

UTMS is a multi-phase academic project that combines:
1. A **university course management system** (Phase 1)
2. **Social features** for students/professors (Phase 2)
3. **Web interface** with client-server architecture (Phase 3)

Key features:
- Course offering/registration
- User profiles and social connections
- Notifications system
- TA recruitment system
- Web-based interface

## 🛠️ Technologies

| Component       | Technologies               |
|-----------------|----------------------------|
| Backend         | C++20, Makefile            |
| Web Interface   | HTML5, CSS3, JavaScript    |
| Architecture   | Client-Server (APHTTP lib) |
| Data Handling  | CSV parsing                |

## 📂 Project Structure


UTMS/
├── server/        # Backend server code
├── src/           # Core logic source files
├── static/        # Static web assets (CSS/JS)
├── template/      # HTML templates
├── utils/         # Utility functions
├── images/        # Image assets
├── Makefile       # Build automation
└── utms.out       # Compiled executable


## 🚀 Installation & Usage

### Prerequisites
- Linux environment
- GCC/G++ supporting C++20
- Make

### Build & Run
```sh
$ make            # Compile the project
$ ./utms.out <majors_csv> <students_csv> <courses_csv> <professors_csv>
```

### Web Interface
After starting the server, access via:
```
http://localhost:8080
```

## 🌟 Key Features

### Phase 1 (Core System)
- User roles: Student, Professor, Admin
- Course management
- User authentication
- Social posting system

### Phase 2 (Enhancements)
- TA recruitment system
- Course channels
- Profile pictures
- Enhanced social features

### Phase 3 (Web Interface)
- Login/Logout system
- Responsive web pages
- Session management
- Interactive forms

## 📚 Documentation


### Data Formats
- **Majors**: `mid,major`
- **Students**: `sid,name,major_id,semester,password`
- **Courses**: `cid,name,credit,prerequisite,majors_id`
- **Professors**: `pid,name,major_id,position,password`


**Note**: This project was developed as part of the Advanced Programming course at University of Tehran.


