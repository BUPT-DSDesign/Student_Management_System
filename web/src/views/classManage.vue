<template>
   <div style="margin:20px">
     <h1>课程管理</h1>
        <el-table :data="tableData" height="250" border style="width: 400px">
            <el-table-column prop="name" label="课程名称" width="180">
            </el-table-column>
            <el-table-column prop="time" label="课程时间" width="180">
            </el-table-column>
        </el-table>
        <el-button type="primary" plain>修改课程</el-button>
        <el-button type="primary" plain>添加课程</el-button>
   </div>
</template>

<script>
export default {
    data() {
        return {
            tableData: [{
                name: '计算机组成原理-03',
                time: '08:00-09:00',
            }, {
                name: '计算机组成原理-02',
                time: '09:00-10:00',
            }, {
                name: '计算机组成原理-04',
                time: '08:00-09:00',
            }, {
                name: '计算机组成原理-01',
                time: '08:00-09:00',
            }, {
                name: '计算机组成原理-08',
                time: '08:00-09:00',
            }, {
                name: '计算机组成原理-06',
                time: '08:00-09:00',
            }, {
                name: '计算机组成原理-07',
                time: '08:00-09:00',
            }]
        }
    }
}
</script>