<template>
    <div>
        <div class="left-container">
            <h2>日程安排表</h2>
            <div class="schedule" >
                <el-table :data="tableData"  :default-sort="{ prop: 'date', order: 'descending' }">
                    <el-table-column prop="date" label="日期" sortable width="180">
                    </el-table-column>
                    <el-table-column prop="time" label="时间" sortable width="180">
                    </el-table-column>
                    <el-table-column prop="name" label="活动名称" :formatter="formatter">
                    </el-table-column>
                </el-table>
            </div>
        </div>
        <div class="right">
            <el-button class="OperationButton" type="primary" icon="el-icon-circle-plus-outline">添加活动</el-button>
            <el-button class="OperationButton" type="primary" icon="el-icon-search">活动查询</el-button>
            <el-button class="OperationButton" type="primary" icon="el-icon-alarm-clock">闹钟提醒</el-button>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            tableData: [{
                date: '2023-02-02',
                time: '15:00',
                name: '班会'
            }, {
                date: '2023-02-04',
                time: '14:00',
                name: '取快递'
            }, {
                date: '2023-02-01',
                time: '14:00',
                name: '计算机网络考试'
            }, {
                date: '2023-02-03',
                time: '14:00',
                name: '出门游玩'
            }]
        }
    },
    methods: {
        formatter(row, column) {
            return row.name;
        }
    }
}
</script>
<style>
h2 {
    margin:15px;
    margin-left:300px;
    font-size: 20px;
}
.left-container{
    float: left;
    width:500px;
    margin-left: 100px;
    margin-top: 50px;
}
.right{
    width:200px;
}
.OperationButton{
    margin:20px
}
</style>