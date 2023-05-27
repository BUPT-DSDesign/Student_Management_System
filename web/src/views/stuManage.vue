<template>
    <div>
        <el-table :data="students" style="width: 100%">
            <el-table-column type="index" width="50"></el-table-column>
            <el-table-column prop="student_id" label="学号" sortable></el-table-column>
            <el-table-column prop="username" label="姓名"></el-table-column>
            <el-table-column prop="signature" label="个性签名"></el-table-column>
            <el-table-column label="课程">
                <template slot-scope="scope">
                    <div v-for="course in scope.row.courses">
                        {{ course.course_name }} ({{ course.is_compulsory ? '必修' : '选修' }})
                    </div>
                </template>
            </el-table-column>
        </el-table>
    </div>
</template>

<script>
import { UserStore } from '@/store/user'
export default {
     created() {
        const getAllStu = async () => {
            const fg = await UserStore.GetAllStuInfo();
            if (fg) {
                this.students = UserStore.allStuInfo;
            } else {
                console.log('error')
            }
        }
        getAllStu();

    },
    data() {
        return {
            students: [
                {
                    student_id: '001',
                    username: '张三',
                    signature: '人生苦短，我用Vue',
                    courses: [
                        {
                            course_name: '数学',
                            is_compulsory: true
                        },
                        {
                            course_name: '英语',
                            is_compulsory: true
                        },
                        {
                            course_name: '编程',
                            is_compulsory: false
                        }
                    ]
                },
                {
                    student_id: '002',
                    username: '李四',
                    signature: '前端大法好',
                    courses: [
                        {
                            course_name: '数学',
                            is_compulsory: true
                        },
                        {
                            course_name: '音乐',
                            is_compulsory: false
                        }
                    ]
                },
                {
                    student_id: '003',
                    username: '王五',
                    signature: '学习使我快乐',
                    courses: [
                        {
                            course_name: '物理',
                            is_compulsory: true
                        },
                        {
                            course_name: '编程',
                            is_compulsory: false
                        }
                    ]
                }
            ]
        }
    }
}
</script>
