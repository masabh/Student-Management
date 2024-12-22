#include <stdio.h>
#include <string.h>

typedef struct result {
    char cc[8];
    double cgpa;
} Result;
typedef struct student
{
	char name[21];
	int id;
	int semester;
	char department[4];
	int number_of_courses;
	Result result[50];
    double average_cgpa;
} Student;

void add_student();
void view();
void search();
void delete();
void edit_result();
void print_student_info(Student student);

void main() {
    int choice;

    while(1) {
        printf("\n\nStudent Management System\n\n");
        printf("Enter your choice: \n \
        1. Add new student. \n \
        2. Display students data. \n \
        3. Search a student by ID \n \
        4. Delete a student from database. \n \
        5. Edit a students result \n \
        0. Exit\n");
        scanf("%d", &choice);
        fflush(stdin);

        if(choice == 0) break;

        switch(choice) {
            case 1:
                add_student();
                break;
            case 2:
                view();
                break;
            case 3:
                search();
                break;
            case 4:
                delete();
                break;
            case 5:
                edit_result();
                break;
        }
    }
}

void add_student() {
    Student student;
    FILE* file;
    file = fopen("students_db.dat", "ab+");
    
    printf("Name: ");
    fgets(student.name, sizeof(student.name), stdin);
    student.name[strcspn(student.name, "\n")] = '\0';

    printf("ID: ");
    scanf("%d", &student.id);
    fflush(stdin);

    printf("Semester: ");
    scanf("%d", &student.semester);
    fflush(stdin);

    printf("Department: ");
    fgets(student.department, sizeof(student.department), stdin);
    student.department[strcspn(student.department, "\n")] = '\0';

    printf("Number of courses: ");
    scanf("%d", &student.number_of_courses);
    fflush(stdin);
    
    for(int i = 0; i<student.number_of_courses; i++) {
        printf("Course %d: \n", i+1);
        printf("Course Code: ");
        fgets(student.result[i].cc, sizeof(student.result[i].cc), stdin);
        student.result[i].cc[strcspn(student.result[i].cc, "\n")] = '\0';

        printf("Obtained CGPA: ");
        scanf("%lf", &student.result[i].cgpa);
        fflush(stdin);
    }

    fwrite(&student, sizeof(Student), 1, file);
    
    fclose(file);
}

void view() {
    Student student;
    FILE* file;
    file = fopen("students_db.dat", "r");
    int curr = 1;
    printf("\nDisplaying students database: \n");
    while(fread(&student, sizeof(Student), 1, file)) {
        printf("\n[%d]\n", curr);
        print_student_info(student);
        curr++;
    }

    fclose(file);
}

void print_student_info(Student student) {
    double average_cgpa, sum = 0;
    
    printf("Name: %s\n", student.name);
    printf("ID: %d\n", student.id);
    printf("Semester: %d\n", student.semester);
    printf("Department: %s\n", student.department);
    printf("\nCourse and CGPA: \n");
    for(int i = 0; i<student.number_of_courses; i++) {
        printf("Course %d: %s\n", i+1, student.result[i].cc);
        printf("CGPA: %.2lf\n", student.result[i].cgpa);
        sum += student.result[i].cgpa;
    }

    average_cgpa = sum / student.number_of_courses;
    printf("Average CGPA: %.2lf\n\n", average_cgpa); 
}

void search() {
    Student student;
    int id, flag = 0;
    FILE* file;
    file = fopen("students_db.dat", "r");

    printf("Enter the ID of the student: ");
    scanf("%d", &id);
    fflush(stdin);

    while(fread(&student, sizeof(Student), 1, file)) {
        if(student.id == id) {
            flag = 1;
            printf("\nStudent Found\n\n");
            print_student_info(student);
        }
    }

    if(flag == 0) printf("\nStudent not found.\n\n");
    fclose(file);
}

void delete() {
    int id, flag = 0;
    Student student;
    FILE* file;
    FILE* temp;
    temp = fopen("temp.dat", "w");
    file = fopen("students_db.dat", "r");

    printf("Enter the ID of the student you want to delete: ");
    scanf("%d", &id);
    fflush(stdin);

    while(fread(&student, sizeof(Student), 1, file)) {
        if(student.id == id) {
            flag = 1;
            continue;
        }
        fwrite(&student, sizeof(Student), 1, temp);
    }
    fclose(temp);
    fclose(file);

    if(flag == 0) {
        printf("Student not found\n");
        remove("temp.dat");
    }
    else {
        remove("students_db.dat");
        rename("temp.dat", "students_db.dat");
        printf("\nDeletion Successful\n\n");
    }
}

void edit_result() {
    int id, flag = 0;
    Student student;
    FILE* temp;
    FILE* file;
    file = fopen("students_db.dat", "r");
    temp = fopen("temp.dat", "w");

    printf("Enter the ID: ");
    scanf("%d", &id);
    fflush(stdin);

    while(fread(&student, sizeof(Student), 1, file)) {
        if(student.id == id) {
            flag = 1;
            printf("Enter the number of courses: ");
            scanf("%d", &student.number_of_courses);
            fflush(stdin);
            for(int i = 0; i<student.number_of_courses; i++) {
                printf("Course no.%d\n", i+1);
                printf("Enter the course code: ");
                fgets(student.result[i].cc, sizeof(student.result[i].cc), stdin);
                student.result[i].cc[strcspn(student.result[i].cc, "\n")] = '\0';

                printf("Enter CGPA: ");
                scanf("%lf", &student.result[i].cgpa);
                fflush(stdin);
            }
        }
        fwrite(&student, sizeof(Student), 1, temp);
    }
    fclose(temp);
    fclose(file);
    remove("students_db.dat");
    rename("temp.dat", "students_db.dat");
    
    if(flag == 1) printf("\nEdited Successfully\n\n");
    else printf("\nStudent was not found\n\n");
}
