#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COURSES 50
#define MAX_STUDENTS 100
#define MAX_ENROLLMENTS 200

/* ========== 数据结构 ========== */
typedef struct {
    char id[10];
    char name[30];
    int credits;
    int capacity;       /* 选课人数上限 */
    int enrolled;       /* 当前已选人数 */
} Course;

typedef struct {
    char id[15];
    char name[20];
} Student;

typedef struct {
    char student_id[15];
    char course_id[10];
} Enrollment;

Course courses[MAX_COURSES];
Student students[MAX_STUDENTS];
Enrollment enrollments[MAX_ENROLLMENTS];
int course_count = 0, student_count = 0, enroll_count = 0;

/* ========== 文件操作 ========== */
void save_courses() {
    FILE *fp = fopen("courses.dat", "wb");
    if (!fp) return;
    fwrite(&course_count, sizeof(int), 1, fp);
    fwrite(courses, sizeof(Course), course_count, fp);
    fclose(fp);
}
void load_courses() {
    FILE *fp = fopen("courses.dat", "rb");
    if (!fp) return;
    fread(&course_count, sizeof(int), 1, fp);
    fread(courses, sizeof(Course), course_count, fp);
    fclose(fp);
}
void save_students() {
    FILE *fp = fopen("students.dat", "wb");
    if (!fp) return;
    fwrite(&student_count, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), student_count, fp);
    fclose(fp);
}
void load_students() {
    FILE *fp = fopen("students.dat", "rb");
    if (!fp) return;
    fread(&student_count, sizeof(int), 1, fp);
    fread(students, sizeof(Student), student_count, fp);
    fclose(fp);
}
void save_enrollments() {
    FILE *fp = fopen("enrollments.dat", "wb");
    if (!fp) return;
    fwrite(&enroll_count, sizeof(int), 1, fp);
    fwrite(enrollments, sizeof(Enrollment), enroll_count, fp);
    fclose(fp);
}
void load_enrollments() {
    FILE *fp = fopen("enrollments.dat", "rb");
    if (!fp) return;
    fread(&enroll_count, sizeof(int), 1, fp);
    fread(enrollments, sizeof(Enrollment), enroll_count, fp);
    fclose(fp);
}
void load_all() { load_courses(); load_students(); load_enrollments(); }
void save_all() { save_courses(); save_students(); save_enrollments(); }

/* ========== 课程管理 ========== */
void add_course() {
    if (course_count >= MAX_COURSES) { printf("课程数已达上限！\n"); return; }
    Course c;
    printf("课程号: "); scanf("%s", c.id);
    printf("课程名: "); scanf("%s", c.name);
    printf("学分: "); scanf("%d", &c.credits);
    printf("选课人数上限: "); scanf("%d", &c.capacity);
    c.enrolled = 0;
    courses[course_count++] = c;
    save_courses();
    printf("课程添加成功！\n");
}
void list_courses() {
    if (course_count == 0) { printf("暂无课程信息。\n"); return; }
    printf("\n%-10s %-20s %-6s %-8s %-8s\n", "课程号", "课程名", "学分", "上限", "已选");
    for (int i = 0; i < course_count; i++)
        printf("%-10s %-20s %-6d %-8d %-8d\n",
               courses[i].id, courses[i].name, courses[i].credits,
               courses[i].capacity, courses[i].enrolled);
}
void modify_course() {
    char id[10]; int found = 0;
    printf("输入要修改的课程号: "); scanf("%s", id);
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, id) == 0) {
            printf("新课程名(原:%s): ", courses[i].name); scanf("%s", courses[i].name);
            printf("新学分(原:%d): ", courses[i].credits); scanf("%d", &courses[i].credits);
            printf("新上限(原:%d): ", courses[i].capacity); scanf("%d", &courses[i].capacity);
            save_courses();
            printf("修改成功！\n"); found = 1; break;
        }
    }
    if (!found) printf("未找到该课程。\n");
}
void delete_course() {
    char id[10], confirm[3]; int found = 0;
    printf("输入要删除的课程号: "); scanf("%s", id);
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, id) == 0) {
            printf("确定删除课程 %s(%s) 吗？(y/n): ", courses[i].id, courses[i].name);
            scanf("%s", confirm);
            if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0) {
                printf("已取消删除。\n"); return;
            }
            /* 同时删除该课程的相关选课记录 */
            int new_enroll_count = 0;
            for (int j = 0; j < enroll_count; j++) {
                if (strcmp(enrollments[j].course_id, id) != 0)
                    enrollments[new_enroll_count++] = enrollments[j];
            }
            enroll_count = new_enroll_count;
            for (int j = i; j < course_count - 1; j++) courses[j] = courses[j + 1];
            course_count--;
            save_all();
            printf("课程已删除！\n"); found = 1; break;
        }
    }
    if (!found) printf("未找到该课程。\n");
}
void query_course_by_id() {
    char id[10]; int found = 0;
    printf("输入课程号: "); scanf("%s", id);
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, id) == 0) {
            printf("课程号:%-10s 课程名:%-20s 学分:%d 上限:%d 已选:%d\n",
                   courses[i].id, courses[i].name, courses[i].credits,
                   courses[i].capacity, courses[i].enrolled);
            found = 1; break;
        }
    }
    if (!found) printf("未找到该课程。\n");
}
void query_course_by_credits() {
    int credits, found = 0;
    printf("输入学分: "); scanf("%d", &credits);
    for (int i = 0; i < course_count; i++) {
        if (courses[i].credits == credits) {
            printf("课程号:%-10s 课程名:%-20s 学分:%d 上限:%d 已选:%d\n",
                   courses[i].id, courses[i].name, courses[i].credits,
                   courses[i].capacity, courses[i].enrolled);
            found = 1;
        }
    }
    if (!found) printf("未找到该学分的课程。\n");
}

/* ========== 学生管理 ========== */
void add_student() {
    if (student_count >= MAX_STUDENTS) { printf("学生数已达上限！\n"); return; }
    Student s;
    printf("学号: "); scanf("%s", s.id);
    printf("姓名: "); scanf("%s", s.name);
    students[student_count++] = s;
    save_students();
    printf("学生添加成功！\n");
}
void list_students() {
    if (student_count == 0) { printf("暂无学生信息。\n"); return; }
    printf("\n%-15s %-10s\n", "学号", "姓名");
    for (int i = 0; i < student_count; i++)
        printf("%-15s %-10s\n", students[i].id, students[i].name);
}
void modify_student() {
    char id[15]; int found = 0;
    printf("输入要修改的学号: "); scanf("%s", id);
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            printf("新姓名(原:%s): ", students[i].name); scanf("%s", students[i].name);
            save_students();
            printf("修改成功！\n"); found = 1; break;
        }
    }
    if (!found) printf("未找到该学生。\n");
}
void delete_student() {
    char id[15], confirm[3]; int found = 0;
    printf("输入要删除的学号: "); scanf("%s", id);
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            printf("确定删除学生 %s(%s) 吗？(y/n): ", students[i].id, students[i].name);
            scanf("%s", confirm);
            if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0) {
                printf("已取消删除。\n"); return;
            }
            /* 同时删除该学生的选课记录 */
            int new_enroll_count = 0;
            for (int j = 0; j < enroll_count; j++) {
                if (strcmp(enrollments[j].student_id, id) != 0)
                    enrollments[new_enroll_count++] = enrollments[j];
            }
            enroll_count = new_enroll_count;
            for (int j = i; j < student_count - 1; j++) students[j] = students[j + 1];
            student_count--;
            save_all();
            printf("学生已删除！\n"); found = 1; break;
        }
    }
    if (!found) printf("未找到该学生。\n");
}
void query_student() {
    char id[15]; int found = 0;
    printf("输入学号: "); scanf("%s", id);
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            printf("学号:%-15s 姓名:%-10s\n", students[i].id, students[i].name);
            /* 显示已选课程 */
            printf("已选课程:\n");
            for (int j = 0; j < enroll_count; j++) {
                if (strcmp(enrollments[j].student_id, id) == 0) {
                    for (int k = 0; k < course_count; k++) {
                        if (strcmp(courses[k].id, enrollments[j].course_id) == 0)
                            printf("  %s %s\n", courses[k].id, courses[k].name);
                    }
                }
            }
            found = 1; break;
        }
    }
    if (!found) printf("未找到该学生。\n");
}

/* ========== 选课操作 ========== */
void select_course() {
    char sid[15], cid[10];
    printf("学号: "); scanf("%s", sid);
    printf("课程号: "); scanf("%s", cid);

    /* 检查学生是否存在 */
    int s_exists = 0;
    for (int i = 0; i < student_count; i++)
        if (strcmp(students[i].id, sid) == 0) { s_exists = 1; break; }
    if (!s_exists) { printf("学生不存在！\n"); return; }

    /* 检查课程是否存在及容量 */
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].id, cid) == 0) {
            if (courses[i].enrolled >= courses[i].capacity) {
                printf("选课失败！课程 %s 已满（上限%d人）\n", cid, courses[i].capacity);
                return;
            }
            /* 检查是否已选过 */
            for (int j = 0; j < enroll_count; j++) {
                if (strcmp(enrollments[j].student_id, sid) == 0 &&
                    strcmp(enrollments[j].course_id, cid) == 0) {
                    printf("你已经选过这门课了！\n"); return;
                }
            }
            enrollments[enroll_count].student_id[0] = '\0';
            strcat(enrollments[enroll_count].student_id, sid);
            enrollments[enroll_count].course_id[0] = '\0';
            strcat(enrollments[enroll_count].course_id, cid);
            enroll_count++;
            courses[i].enrolled++;
            save_all();
            printf("选课成功！\n");
            return;
        }
    }
    printf("课程不存在！\n");
}
void cancel_course() {
    char sid[15], cid[10];
    printf("学号: "); scanf("%s", sid);
    printf("课程号: "); scanf("%s", cid);

    for (int i = 0; i < enroll_count; i++) {
        if (strcmp(enrollments[i].student_id, sid) == 0 &&
            strcmp(enrollments[i].course_id, cid) == 0) {
            for (int j = i; j < enroll_count - 1; j++) enrollments[j] = enrollments[j + 1];
            enroll_count--;
            /* 减少课程已选人数 */
            for (int k = 0; k < course_count; k++)
                if (strcmp(courses[k].id, cid) == 0) courses[k].enrolled--;
            save_all();
            printf("取消选课成功！\n");
            return;
        }
    }
    printf("未找到该选课记录。\n");
}

/* ========== 菜单 ========== */
void main_menu() {
    printf("\n========== 学生选课管理系统 ==========\n");
    printf(" 1. 添加课程\n");
    printf(" 2. 浏览课程\n");
    printf(" 3. 修改课程\n");
    printf(" 4. 删除课程\n");
    printf(" 5. 按课程号查询课程\n");
    printf(" 6. 按学分查询课程\n");
    printf(" 7. 添加学生\n");
    printf(" 8. 浏览学生\n");
    printf(" 9. 修改学生\n");
    printf("10. 删除学生\n");
    printf("11. 按学号查询学生\n");
    printf("12. 学生选课\n");
    printf("13. 取消选课\n");
    printf(" 0. 退出系统\n");
    printf("======================================\n");
    printf("请选择: ");
}

int main() {
    load_all();
    int op;
    while (1) {
        main_menu();
        if (scanf("%d", &op) != 1) { while (getchar() != '\n'); continue; }
        switch (op) {
            case 1: add_course(); break;
            case 2: list_courses(); break;
            case 3: modify_course(); break;
            case 4: delete_course(); break;
            case 5: query_course_by_id(); break;
            case 6: query_course_by_credits(); break;
            case 7: add_student(); break;
            case 8: list_students(); break;
            case 9: modify_student(); break;
            case 10: delete_student(); break;
            case 11: query_student(); break;
            case 12: select_course(); break;
            case 13: cancel_course(); break;
            case 0: printf("感谢使用，再见！\n"); return 0;
            default: printf("无效选项，请重新输入。\n");
        }
    }
}
