<template>
    <div style="margin:20px">
        <h1>课程管理</h1>
        <el-table :data="tableData" height="250" border style="width: 400px" @row-click="clickcell">
            <el-table-column prop="classname" label="课程名称" width="180" >
            </el-table-column>
            <el-table-column prop="classtime" label="课程时间" width="180">
            </el-table-column>
        </el-table>
        <el-button type="primary" plain>修改课程</el-button>
        <el-button type="primary" plain>添加课程</el-button>

         <!-- 点击弹窗 -->
            <el-dialog title="课程详情" :visible.sync="dialogVisible1" width="30%" :before-close="handleClose">
                <el-card class="box-card">
                    <div class="course_name">课程名称：{{ clickedClassData.course_name }}</div>
                    <div class="course_time">课程时间：{{ clickedClassData.classTime }}</div>
                    <div class="course_address">课程地点：{{ clickedClassData.classroom }}</div>
                </el-card>
                <span slot="footer" class="dialog-footer">
                    <el-button @click="dialogVisible1 = false">取 消</el-button>
                    <el-button type="primary" @click="dialogVisible1 = false">确 定</el-button>
                </span>
            </el-dialog>
    </div>
</template>

<script>
import { CourseStore } from '@/store/course';

const startTimeMap = {
    0: "8:00",
    1: "9:00",
    2: "10:00",
    3: "11:00",
    4: "13:00",
    5: "14:00",
    6: "15:00",
    7: "16:00",
    8: "17:00",
};
const endTimeMap = {
    0: "9:00",
    1: "10:00",
    2: "11:00",
    3: "12:00",
    4: "14:00",
    5: "15:00",
    6: "16:00",
    7: "17:00",
    8: "18:00",
}
export default {
    data() {
        return {
            classData: [],
            tableData: [],
            clickedClass: '',  //当前点击的单元格的课程名称
            clickedClassData: {},  //记录当前点击单元格课程的信息（对象类型
            dialogVisible1: false, //点击课程弹窗的可见性
            dialogVisible2: false, //添加课程弹窗的可见性
        }
    },
    created () {
        const getTable = async () => {
            const fg = await CourseStore.GetCourseTable();
            if (fg) {
                this.classData = CourseStore.courseList
                //添加一个属性以便格式化课程时间
                for (let i = 0; i < this.classData.length; i++) {
                    let startTime = startTimeMap[this.classData[i].section_list[0] % 9];
                    let endTime = endTimeMap[this.classData[i].section_list.slice(-1) % 9];
                    this.classData[i]['classTime'] = startTime+'-'+endTime;

                }
                console.log(this.classData);
                for (let i = 0; i < this.classData.length; i++) {
                    this.tableData.push({
                        classname: this.classData[i].course_name,
                        classtime: this.classData[i].classTime,
                    })
                }
            } else {
                console.log('error')
            }
        }
        getTable();
       
    },
    methods: {
        clickcell(e) {
            this.clickedClass = e.classname;
            // 循环遍历找到数据库中该课程的所有信息
            for (let i = 0; i < this.classData.length; i++) {
                if (this.classData[i].course_name == this.clickedClass) {
                    //如果有该课程，更新curClassData
                    this.clickedClassData = this.classData[i];
                    console.log(this.clickedClassData)
                }
            }
            //点击弹窗
            this.dialogVisible1 = true;
        },
        // 关闭弹窗
        handleClose(done) {
            done();
        },
    },
}
</script>