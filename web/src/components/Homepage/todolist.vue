<template>
    <div class="container">
        <div class="head">
            <h2>ToDoList</h2>
            <input type="text" placeholder="添加ToDo" v-model="todo" @keyup.enter="addTodo">
            <button @click="clearData">clear</button>
        </div>
        <div class="content">
            <h2>待办事项
                <span>{{ todoLen }}</span>
            </h2>
            <ol>
                <li v-for="(item, index) in todoList" :key="index" v-if="item.done === false">
                    <input type="checkbox" @change="changeTodo(index, true)">
                    <p>{{ item.todo }}</p>
                    <a @click="deleteTodo(index, true)">-</a>
                </li>
            </ol>
            <h2>已完成
                <span>{{ todoList.length - todoLen }}</span>
            </h2>
            <ol class="done">
                <li v-for="(item, index) in todoList" :key="index" v-if="item.done === true">
                    <input type="checkbox" @change="changeTodo(index, false)" checked='checked'>
                    <p>{{ item.todo }}</p>
                    <a @click="deleteTodo(index, false)">-</a>
                </li>
            </ol>
        </div>
    </div>
</template>

<script>
import * as Utils from '../../utils/request'
export default {
    name: 'to-do-list',
    data() {
        return {
            todo: '',
            todoList: [],
            todoLen: 0,
        }
    },
    methods: {
        initTodo() {

        },
        addTodo() {

        },
        changeTodo(index, done) {

        },
        deleteTodo(index, done) {
        },
        clearData() {
            localStorage.clear()
            this.todoList = []
            this.todoLen = 0
        },

    },
    mounted() {
        this.initTodo()
    }
}
</script>

<style>
h1,
h2,
h3,
h4,
h5,
h6 {
    color: black;
}

.content {
    margin: 0 auto;
    width: 600px;
}


.content h2 {
    text-align: left;
    position: relative;
}

.content h2 span {
    position: absolute;
    top: 5px;
    right: 5px;
    display: inline-block;
    padding: 0 5px;
    height: 20px;
    border-radius: 20px;
    background-color: #55BB8E;
    font-size: 14px;
    color: #FFF;
    line-height: 22px;
    text-align: center;
}

.content ol {
    list-style: none;
    padding: 0;
}

.content ol li {
    cursor: move;
    height: 32px;
    background-color: #FFF;
    position: relative;
    margin-bottom: 10px;
    padding: 0 45px;
    border-radius: 3px;
    border-left: 5px solid #55BB8E;
}

.content ol li input {
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    top: 2px;
    left: 10px;
}

.content ol li p {
    /* display: inline-block; */
    margin: 0;
    padding: 5px;
    text-align: left;

}

.content ol li a {
    display: inline-block;
    position: absolute;
    top: 2px;
    right: 5px;
    width: 14px;
    height: 12px;
    border-radius: 14px;
    background-color: #CCC;
    line-height: 14px;
    border: 6px double #FFF;
    text-align: center;
    color: #FFF;
    font-size: 14px;
    cursor: pointer;
}

.head input {
    width: 500px;
    margin-right: 30px;
    padding: 10px;
    border: 1px solid #EFEFEF;
}

.head button {
    background-color: #55BB8E;
    border-radius: 5px;
    border: 1px solid transparent;
    padding: 10px;
    color: #FFF;
    outline: none;

}

input[type=checkbox]:checked {
    background-color: #55BB8E;
}

.done li {
    opacity: 0.5
}

.done p {
    text-decoration: line-through;
}

input[type=text]:focus {
    outline: none;
}

input[type=checkbox]:checked {
    background-color: #55BB8E;
}</style>