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
            students: []
        }
    }
}
</script>
