<template>
    <div style="margin:20px">
        <h1>课程管理</h1>
        <el-table :data="tableData" height="250" border style="width: 400px">
            <el-table-column prop="classname" label="课程名称" width="180">
            </el-table-column>
            <el-table-column prop="classtime" label="课程时间" width="180">
            </el-table-column>
        </el-table>
        <el-button type="primary" plain>修改课程</el-button>
        <el-button type="primary" plain>添加课程</el-button>
    </div>
</template>

<script>
import { CourseStore } from '@/store/course';
// import { start } from 'repl';
// import { clearScreenDown } from 'readline';
const startTimeMap = {
    1: "8:00",
    2: "9:00",
    3: "10:00",
    4: "11:00",
    5: "13:00",
    6: "14:00",
    7: "15:00",
    8: "16:00",
    9: "17:00",
};
const endTimeMap = {
    1: "9:00",
    2: "10:00",
    3: "11:00",
    4: "12:00",
    5: "14:00",
    6: "15:00",
    7: "16:00",
    8: "17:00",
    9: "18:00",
}
export default {
    mounted() {
        // 当点击侧边栏的课内信息时, 会向后端发送请求, 后端返回课程表
        const getTable = async () => {
            const fg = await CourseStore.GetCourseTable();
        }
        getTable();
        this.classData = CourseStore.courseList
        //添加一个属性以便格式化课程时间
        for (let i = 0; i < this.classData.length; i++) {
            let startTime = startTimeMap[this.classData[i].section_list[0] % 10];
            let endTime = endTimeMap[this.classData[i].section_list.slice(-1) % 10];
            this.classData[i]['classTime'] = startTime+'-'+endTime;

        }
        console.log(this.classData);
        for (let i = 0; i < this.classData.length; i++) {
            this.tableData.push({
                classname: this.classData[i].course_name,
                classtime: this.classData[i].classTime,

            })
        }
    },
    data() {
        return {
            classData: [],
            tableData: [],
        }
    }
}
</script>