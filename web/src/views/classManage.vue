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
// import { clearScreenDown } from 'readline';
const timemap = {
    1: "8:00-9:00",
    2: "9:00-10:00",
    3: "10:00-11:00",
    4: "11:00-12:00",
    5: "12:00-13:00",
    6: "13:00-14:00",
    7: "14:00-15:00",
    8: "15:00-16:00",
    9: "16:00-17:00",
    10: "17:00-18:00",
}
export default {
    beforeMount() {
         // 当点击侧边栏的课内信息时, 会向后端发送请求, 后端返回课程表
        const getTable = async () => {
            const fg = await CourseStore.GetCourseTable();
        }
        getTable();
        this.classData = CourseStore.courseList
        //添加一个属性以便格式化课程时间
        for (let i = 0; i < this.classData.length; i++){
            for (let j = 0; j < this.classData[i].section_list.length; j++){
                let classtime = timemap[this.classData[i].section_list[j]%10];
                this.classData[i]['classtime']= classtime;
            }
        }
        console.log(this.classData);
        for (let i = 0; i < this.classData.length; i++){
            this.tableData.push({
                classname: this.classData[i].course_name,
                classtime:this.classData[i].classtime,
                
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