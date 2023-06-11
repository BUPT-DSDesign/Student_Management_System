
```
server
├─ algorithm
│  ├─ check_conflict
│  │  ├─ check_activity.go
│  │  └─ check_activity_test.go
│  ├─ description.md
│  ├─ dijkstra
│  │  └─ dijkstra.go
│  ├─ heap
│  │  └─ heap.go
│  ├─ initialize
│  │  └─ initialize.go
│  ├─ my_sort
│  │  ├─ impl_sorter.go
│  │  ├─ my_sort_test.go
│  │  └─ quick_sort.go
│  └─ tsp
│     ├─ aco.go
│     ├─ ga.go
│     ├─ initialize.go
│     └─ tsp.go
├─ database.exe
├─ docs
│  ├─ description.md
│  ├─ docs.go
│  ├─ swagger.json
│  └─ swagger.yaml
├─ go.mod
├─ go.sum
├─ handler
│  ├─ activity_handler
│  │  ├─ add.go
│  │  ├─ delete.go
│  │  ├─ get_info.go
│  │  └─ query_by_time.go
│  ├─ course_handler
│  │  ├─ add.go
│  │  ├─ all.go
│  │  ├─ delete.go
│  │  ├─ search.go
│  │  ├─ select.go
│  │  ├─ selective.go
│  │  ├─ table.go
│  │  └─ update.go
│  ├─ description.md
│  ├─ handler.go
│  ├─ log_handler
│  │  ├─ add.go
│  │  ├─ delete.go
│  │  └─ info.go
│  ├─ navigate_handler
│  │  ├─ facility.go
│  │  ├─ search.go
│  │  └─ tsp.go
│  ├─ polling_handler
│  │  ├─ is_activity_arrive.go
│  │  └─ is_course_arrive.go
│  └─ user_handler
│     ├─ all_info.go
│     ├─ edit_signature.go
│     ├─ info.go
│     ├─ login.go
│     ├─ register.go
│     └─ upload_avatar.go
├─ main.go
├─ middleware
│  ├─ description.md
│  ├─ jwt.go
│  ├─ sha.go
│  └─ tls.go
├─ model
│  ├─ dao
│  │  ├─ activity_operate.go
│  │  ├─ course_operate.go
│  │  ├─ create_table.go
│  │  ├─ create_table_test.go
│  │  ├─ enter.go
│  │  ├─ initialize.go
│  │  ├─ log_operate.go
│  │  └─ user_operate.go
│  └─ entity
│     ├─ common
│     │  ├─ request.go
│     │  └─ response.go
│     ├─ description.md
│     ├─ entity.go
│     └─ system
│        ├─ activity_info.go
│        ├─ building.go
│        ├─ course_info.go
│        ├─ course_section.go
│        ├─ course_week.go
│        ├─ facility_info.go
│        ├─ log_info.go
│        ├─ navigate_info.go
│        ├─ student_course.go
│        └─ user_info.go
├─ resources
│  ├─ edge - 副本.csv
│  ├─ edge.csv
│  ├─ node - 副本.csv
│  └─ node.csv
├─ router
│  ├─ description.md
│  └─ router.go
├─ secret
│  ├─ key.pem
│  ├─ test.key
│  ├─ test.pem
│  └─ test.pfx
├─ service
│  ├─ activity_service
│  │  ├─ add.go
│  │  ├─ delete.go
│  │  ├─ enter.go
│  │  ├─ get_info.go
│  │  └─ query_by_time.go
│  ├─ course_service
│  │  ├─ add.go
│  │  ├─ all.go
│  │  ├─ delete.go
│  │  ├─ enter.go
│  │  ├─ search.go
│  │  ├─ select.go
│  │  ├─ selective.go
│  │  ├─ table.go
│  │  └─ update.go
│  ├─ description.md
│  ├─ log_service
│  │  ├─ add.go
│  │  ├─ delete.go
│  │  ├─ enter.go
│  │  └─ info.go
│  ├─ navigate_service
│  │  ├─ enter.go
│  │  ├─ facility.go
│  │  ├─ navigate.go
│  │  └─ tsp.go
│  ├─ polling_service
│  │  ├─ enter.go
│  │  ├─ is_activity_arrive.go
│  │  └─ is_course_arrive.go
│  └─ user_service
│     ├─ all_info.go
│     ├─ edit_signature.go
│     ├─ enter.go
│     ├─ info.go
│     ├─ login.go
│     ├─ register.go
│     ├─ register_test.go
│     └─ upload_avatar.go
├─ sms
│  ├─ activity_info.table
│  ├─ activity_info.tbinfo
│  ├─ course_info.table
│  ├─ course_info.tbinfo
│  ├─ course_section.table
│  ├─ course_section.tbinfo
│  ├─ course_week.table
│  ├─ course_week.tbinfo
│  ├─ log_info.table
│  ├─ log_info.tbinfo
│  ├─ student_course.table
│  ├─ student_course.tbinfo
│  ├─ user_info.table
│  └─ user_info.tbinfo
└─ utils
   ├─ array_ops.go
   ├─ array_ops_test.go
   ├─ description.md
   ├─ jwt.go
   ├─ sha.go
   ├─ snowflake.go
   ├─ snowflake_test.go
   ├─ time.go
   └─ type_transform.go

```