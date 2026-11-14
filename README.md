# AcadERP — Academic Records & Graduation Audit System

**Group ID:** 63
**Language:** C++17

## 1. Project Overview

AcadERP is a command-driven academic records and graduation audit system. It manages students, courses, registrations, grades, thesis progress, GPA calculations, graduation requirements, and course eligibility.

The program reads commands from standard input and prints query results and errors in the required format.

## 2. Build and Execution

### Requirements
- Linux environment
- GNU C++ compiler supporting C++17
- Make

### Build

```bash
make
```

This produces the executable `acaderp`.

To clean and rebuild:

```bash
make clean
make
```

### Run interactively

```bash
./acaderp
```

Enter AcadERP commands through standard input. Press `Ctrl+D` to finish.

### Run using an input file

```bash
./acaderp < tests/T01.in
```

### Run the test suite

```bash
./run_tests.sh
```

The test script executes the available `Tnn.in` files and compares actual output against their corresponding `Tnn.out` files.

## 3. Design and Class Structure

The implementation uses object-oriented design, encapsulation, inheritance, and runtime polymorphism.

### Grade
An immutable value object representing a grade symbol. Grades are created through a factory that validates the symbol. It provides operations such as `isPass()`, `isFinal()`, and `pointsHundredths()`.

### Course hierarchy
- `Course`: Abstract base class containing course information, prerequisites, anti-requisites, and category.
- `RegularCourse`: Supports regular letter grades and incomplete grades.
- `SuCourse`: Supports satisfactory, unsatisfactory, and incomplete grades.
- `ThesisCourse`: Represents thesis courses and their progress-grade rules.

### Student and attempt management
- `Student`: Stores student information and course attempts.
- `Attempt`: Represents a regular or SU course registration, including its term, attempt number, and grade.
- `ThesisRecord`: Maintains the thesis lifecycle, progress grades, progress terms, and final grade.

### Rules and application logic
- `ProgramRules`: Stores programme-level graduation and registration requirements.
- `Registrar`: Provides the central operations for registration, grade submission, thesis conversion, GPA, graduation, eligibility, and transcript queries.
- `CommandParser`: Parses and executes input commands.
- `Exceptions`: Provides typed exceptions for invalid commands and domain-rule violations.

## 4. Supported Commands

| Command | Purpose |
|---|---|
| `PROGRAM` | Configure programme requirements |
| `COURSE` | Add a course |
| `STUDENT` | Add a student |
| `REGISTER` | Register a student for a course |
| `GRADE` | Submit a regular, SU, or thesis progress grade |
| `CONVERT` | Convert an eligible thesis to a letter grade |
| `GPA` | Calculate the student's GPA |
| `AVG` | Calculate the average grade |
| `GRAD` | Check graduation requirements |
| `ELIGIBLE` | Check course registration eligibility |
| `TRANSCRIPT` | Display the student's academic record |

### Course definition example

```text
COURSE CS101 REGULAR 3 CORE
COURSE CS201 REGULAR 3 ELECTIVE PRE=CS101:B
COURSE NSS SU 0 MANDATORY_SU
COURSE TH501 THESIS 6 OPEN
```

### Student and registration example

```text
STUDENT 1001 Rahul
REGISTER 1001 CS101 2026-1
GRADE 1001 CS101 2026-1 A
```

### Query example

```text
GPA 1001
AVG 1001
GRAD 1001
TRANSCRIPT 1001
```

## 5. Important Domain Rules

### Grades
- Regular courses accept A, A-, B, B-, C, C-, D, E, F, and I.
- SU courses accept S, X, and I.
- Thesis progress grades are S and X.
- An incomplete grade I may be replaced by a final grade.
- A final regular or SU grade cannot be changed.

### Course attempts
- A new attempt is allowed after a failed final grade.
- One grade-improvement attempt is allowed for a regular course whose latest final grade is C- or D.
- The counting attempt is the latest attempt with a final grade.

### Thesis lifecycle

A thesis follows these states:

1. REGISTERED
2. IN_PROGRESS
3. AT_RISK
4. TERMINATED
5. FINAL

A progress grade S moves the thesis to IN_PROGRESS. An X moves it to AT_RISK. Two consecutive X progress submissions terminate the thesis. A terminated thesis cannot receive further grades, but the student may register again.

Only an IN_PROGRESS thesis can be converted to a final letter grade.

Progress grades must be submitted in strictly increasing terms after the registration term.

### GPA
- GPA uses counting attempts of regular courses and final theses.
- Failed grades are included.
- SU courses, incomplete grades, pending registrations, and unconverted theses are excluded.
- GPA is calculated using integer arithmetic and rounded half-up to two decimal places.

### Eligibility
Eligibility checks are evaluated in this order:

- E1: Student and course existence
- E2: Already passed
- E3: Duplicate open registration
- E4: Prerequisite requirements
- E5: Anti-requisite requirements
- E6: Maximum term credit load
- E7: Thesis eligibility requirements

## 6. Transcript Format

Regular and SU attempts are displayed in this format:

```text
COURSE_CODE[TERM,#ATTEMPT_NUMBER]=GRADE
```

Thesis records are displayed in this format:

```text
COURSE_CODE[TERM]=STATE:STATE_NAME,PROGRESS:GRADE1,GRADE2
```

A converted thesis also includes:

```text
,FINAL_GRADE:LETTER
```

Example:

```text
TRANSCRIPT 113 CS101[2026-1,#1]=A TH501[2026-1]=STATE:TERMINATED,PROGRESS:X,X TH501[2027-1]=STATE:IN_PROGRESS,PROGRESS:S
```

## 7. Error Handling

Domain and input errors are represented using typed exceptions derived from `ErpException`.

The command loop reports errors in this format:

```text
ERROR <line_number> <ERROR_CODE>
```

The program continues processing subsequent commands after a handled command error.

## 8. Test Suite

The project includes 16 input/output test pairs:

- T01–T10: Original test cases.
- T11: Thesis progress term ordering.
- T12: Rejection of grades after thesis termination.
- T13: Re-registration after thesis termination.
- T14: Graduation R7 handling of replaced incomplete grades.
- T15: Prerequisite minimum-grade validation.
- T16: Maximum term credit limit.

Run all tests with:

```bash
./run_tests.sh
```

## 9. Assumptions and Implementation Decisions

- Student names are supplied as a single token.
- Academic terms use the `YYYY-1`, `YYYY-2`, and `YYYY-3` format.
- A prerequisite without an explicit minimum grade uses D for a regular course and S for an SU course.
- Prerequisite validation uses the counting attempt of the prerequisite course.
- Thesis progress submissions must be strictly later than the registration term and the previous progress term.
- A thesis may be registered again after termination.
- Transcript output includes all stored regular/SU attempts and thesis records, rather than only counting attempts.
- Blank lines and comment lines are ignored while retaining their position in input line numbering.

## 10. Performance and Scalability

The system uses STL containers to manage students, courses, attempts, and thesis records.

The main operations have different costs depending on the number of stored records:

- Student and course lookup use associative containers.
- Finding a student's latest or counting attempt may require scanning that student's attempts.
- Eligibility checks scan relevant attempts and prerequisites.
- GPA and graduation calculations process the student's academic records.
- Transcript generation visits the student's attempts and thesis records.

For workloads involving more than 3,000 students, actual performance depends on the number of courses, attempts, prerequisites, and commands processed.

Potential optimizations include indexing attempts by course code, maintaining direct student-to-thesis indexes, caching counting attempts, and avoiding repeated full scans.

No benchmark result is claimed without measurement on a representative dataset.